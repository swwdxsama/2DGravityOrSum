#pragma once
#include <cmath>

enum colors { RED, BLUE, GREEN };

typedef struct Obj Obj;
typedef struct Node Node;
typedef struct List List;


struct Obj {
    float radius;
    float x;
    float y;
    float dx;
    float dy;
    float mass; // mass in kgs
    colors color;
    bool main;
    //List* prevs;
};

struct Node {
    Node* next;
    Node* prev;
    Obj* obj;
};

struct List {
    Node* first;
    Node* last;
    int size;
};

typedef struct nn {
    struct nn* next;
    struct nn* prev;
    Obj* object;
}objNode;

typedef struct {
    objNode* first;
    objNode* last;
}objList;


objList createObjList() {
    objList ret;
    ret.first = NULL;
    ret.last = NULL;
    return ret;
}

objNode* createNode(Obj* object) {
    objNode* node = (objNode*)malloc(sizeof(objNode));
    node->object = object;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void addObject(objList* list, objNode* node) {
    if (list->first == NULL) {
        list->first = node;
        list->last = node;
    }
    else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }
}

Obj* createObject(float radius, float x, float y, float dx, float dy, float mass, colors color, bool main) {
    Obj* obj = (Obj*)malloc(sizeof(Obj));
    obj->radius = radius;
    obj->x = x; obj->y = y; obj->dx = dx; obj->dy = dy;
    obj->mass = mass; obj->color = color; obj->main = main;
    return obj;
}

List* createList(){
    List* list = (List*)malloc(sizeof(List));
    list->size = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

void listAdd(List* list, Obj obj) {
    if (list->size == 0) {
        Node* first = (Node*)malloc(sizeof(Node));
        first->obj = &obj;
        first->next = NULL;
        first->prev = NULL;
        list->first = first;
        list->last = first;
        list->size++;
    }
    else if (list->size == 60) {
        Node* bem = list->last;
        list->last = list->last->prev;
        list->last->next = NULL;
        free(bem);
        Node* bam = (Node*)malloc(sizeof(Node));
        bam->obj = &obj;
        bam->next = list->first;
        bam->prev = NULL;
        list->first->prev = bam;
        list->first = bam;
        list->size++;
    }
    else {
        Node* bem = (Node*)malloc(sizeof(Node));
        bem->obj = &obj;
        bem->next = list->first;
        bem->prev = NULL;
        list->first->prev = bem;
        list->first = bem;
        list->size++;
    }
}

constexpr float EPS = 1e-6f;
constexpr float G = 6.6743e-11;

void fillCircle(SDL_Renderer* ren, float cx, float cy, float r, colors color) {
    int x = r, y = 0, err = 1 - r;
    switch (color) {
    case RED:
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        break;
    case BLUE:
        SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
        break;
    case GREEN:
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
        break;
    }
    while (x >= y) {
        SDL_RenderLine(ren, cx - x, cy + y, cx + x, cy + y);
        SDL_RenderLine(ren, cx - y, cy + x, cx + y, cy + x);
        SDL_RenderLine(ren, cx - x, cy - y, cx + x, cy - y);
        SDL_RenderLine(ren, cx - y, cy - x, cx + y, cy - x);

        y++;
        if (err < 0)
            err += 2 * y + 1;
        else {
            x--;
            err += 2 * (y - x) + 1;
        }
    }
}

void drawObject(SDL_Renderer* renderer, Obj object) {
    fillCircle(renderer, object.x, object.y, object.radius, object.color);
    /*
    if (object.prevs->size > 0) {
        Node* it = object.prevs->first;
        float radius = 0.9;
        fillCircle(renderer, it->obj->x, it->obj->y, it->obj->radius*radius, it->obj->color);
        while (it->next != NULL) {
            radius = radius - 0.00166;
            it = it->next;
            fillCircle(renderer, it->obj->x, it->obj->y, it->obj->radius * radius, it->obj->color);
        }
    }
    */
}

inline bool floatEqual(float a, float b) {
    return std::fabs(a - b) < EPS;
}

inline bool operator==(const Obj& lhs, const Obj& rhs) {
    return floatEqual(lhs.radius, rhs.radius) &&
        floatEqual(lhs.x, rhs.x) &&
        floatEqual(lhs.y, rhs.y) &&
        floatEqual(lhs.dx, rhs.dx) &&
        floatEqual(lhs.dy, rhs.dy) &&
        lhs.color == rhs.color;
}

float calculateDistance(Obj object1, Obj object2) {
    return (sqrt(abs(object1.x-object2.x)*abs(object1.x-object2.x) + abs(object1.y-object2.y)* abs(object1.y - object2.y)));
}

float calculateGravitationalPull(Obj object1, Obj object2) {
    float r = calculateDistance(object1, object2);
    r = r * r;
    return (G * object1.mass * object2.mass)/r;
}

void normalizeVector(float* x, float* y) {
    float d = sqrt(*x**x + *y**y);
    *x = *x / d;
    *y = *y / d;
}

void applyGravitationalPull(Obj &object1, Obj &object2) {
    float F = calculateGravitationalPull(object1, object2);
    float x1 = object2.x - object1.x;
    float y1 = object2.y - object1.y;
    normalizeVector(&x1, &y1);
    object1.dx += F * x1;
    object1.dy += F * y1;
    float x2 = object1.x - object2.x;
    float y2 = object1.y - object2.y;
    normalizeVector(&x2, &y2);
    object2.dx += F * x2;
    object2.dy += F * y2;
}

inline bool operator!=(const Obj& lhs, const Obj& rhs) {
    return !(lhs == rhs);
}

bool detectCollision(Obj first, Obj second) {
    float distance = sqrt(abs(first.x - second.x) * abs(first.x - second.x) + abs(first.y - second.y) * abs(first.y - second.y));
    if (distance < (first.radius + second.radius)) return true;
    return false;
}

int wallColision(Obj& object) {
    if (object.x + object.radius >= 1000) return 1;
    if (object.x - object.radius <= 0) return 2;
    if (object.y + object.radius >= 1000) return 3;
    if (object.y - object.radius <= 0) return 4;
    return 0;
}

void applyMovement(Obj& object) {
    object.x += object.dx;
    object.y += object.dy;
}

void applyTransformation(Obj& mainObj, Obj& target) {
    target.x -= mainObj.dx;
    target.y -= mainObj.dy;
}

void resolveWallColision(Obj& object) {
    int b = wallColision(object);
    switch (b) {
    case 1:
        object.dx = -object.dx;
        object.x = 1000 - object.radius;
        break;
    case 2:
        object.dx = -object.dx;
        object.x = 0 + object.radius;
        break;
    case 3:
        object.dy = -object.dy;
        object.y = 1000 - object.radius;
        break;
    case 4:
        object.dy = -object.dy;
        object.y = 0 + object.radius;
        break;
    }
}

void resolveCollision(Obj& a, Obj& b) {
    float nx = b.x - a.x;
    float ny = b.y - a.y;
    float dist = sqrtf(nx * nx + ny * ny);
    if (dist == 0) return;

    // Normalize normal vector
    nx /= dist;
    ny /= dist;

    // --- Step 1: Separate circles ---
    float overlap = (a.radius + b.radius) - dist;
    if (overlap > 0) {
        a.x -= nx * (overlap / 2.0f);
        a.y -= ny * (overlap / 2.0f);
        b.x += nx * (overlap / 2.0f);
        b.y += ny * (overlap / 2.0f);
    }

    // --- Step 2: Calculate velocities along the normal ---
    float va = a.dx * nx + a.dy * ny;
    float vb = b.dx * nx + b.dy * ny;

    // If they are moving apart, no collision response needed
    if (va - vb <= 0) return;

    // --- Step 3: Swap normal components (equal mass) ---
    float temp = va;
    va = vb;
    vb = temp;

    // --- Step 4: Recombine into full velocity vectors ---
    float tax = a.dx - nx * (a.dx * nx + a.dy * ny);
    float tay = a.dy - ny * (a.dx * nx + a.dy * ny);

    float tbx = b.dx - nx * (b.dx * nx + b.dy * ny);
    float tby = b.dy - ny * (b.dx * nx + b.dy * ny);

    a.dx = tax + nx * va;
    a.dy = tay + ny * va;
    b.dx = tbx + nx * vb;
    b.dy = tby + ny * vb;
}

void applyMovement(Obj& object, float dx, float dy) {
    object.x += dx;
    object.y += dy;
}