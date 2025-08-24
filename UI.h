#pragma once

int printObjects(objList& list) {
    system("CLS");
    objNode* nod = list.first;
    int i = 1;
    while (nod != NULL) {
        if (nod == NULL) break;
        printf("OBJECT %d AT %f %f\n", i, nod->object->x, nod->object->y);
        i++;
        nod = nod->next;
    }
    return i;
}

void viewObjects(objList &list){
    printObjects(list);
    char inp[256];
    printf("Write something to return:\n");
    scanf_s("%s", inp, sizeof(inp));
    return;
}

void editPosition(objNode* nod) {
    printf("\nintroduce x: ");
    scanf_s("%f", &nod->object->x);
    printf("\nintroduce y: ");
    scanf_s("%f", &nod->object->y);

}

void editVelocity(objNode* nod) {
    printf("\nintroduce dx: ");
    scanf_s("%f", &nod->object->dx);
    printf("\nintroduce dy: ");
    scanf_s("%f", &nod->object->dy);

}

void editColor(objNode* nod) {
    printf("\nchoose one of the colors:\n1.)RED\n2.)GREEN\n3.)BLUE\n");
    int ch;
    scanf_s("%d", &ch);
    switch (ch) {
    case 1:
        nod->object->color = RED;
        break;
    case 2:
        nod->object->color = GREEN;
        break;
    case 3:
        nod->object->color = BLUE;
        break;
    }

}

void editMass(objNode* nod) {
    printf("\nintroduce mass: ");
    scanf_s("%f", &nod->object->mass);
}

void editRadius(objNode* nod) {
    printf("\nintroduce radius: ");
    scanf_s("%f", &nod->object->radius);
}

void editObject(objList& list) {
    int i = printObjects(list);
    int num;

    printf("Enter The Number Of The Object You Want To Edit: ");
    scanf_s("%d", &num);
    if (num > i || num < 1) return;
    objNode* nod = list.first;
    i = 1;
    while (nod != NULL && i != num) {
        printf("OBJECT %d AT %f %f\n", i, nod->object->x, nod->object->y);
        i++;
    }
    if (nod != NULL) {
        printf("Choose what to edit : \n1.)Position\n2.)Velocity\n3.)Color\n4.)Mass\n5.)Radius\n");
        int ch;
        scanf_s("%d", &ch);
        switch (ch) {
        case 1:
            editPosition(nod);
            break;
        case 2:
            editVelocity(nod);
            break;
        case 3:
            editColor(nod);
            break;
        case 4:
            editMass(nod);
            break;
        case 5:
            editRadius(nod);
            break;

        }
    }
}

void addObject(objList& list) {
    objNode* nod = (objNode*)malloc(sizeof(objNode));
    nod->next = NULL;
    nod->object = (Obj*)malloc(sizeof(Obj));
    if (list.first == NULL) nod->object->main = true;
    else nod->object->main = false;
    editPosition(nod);
    editVelocity(nod);
    editColor(nod);
    editMass(nod);
    editRadius(nod);
    addObject(&list, nod);
}

void viewObject(objList& list) {}



objList startUI() {
    objList toRet = createObjList();
    std::string input;
    int stop = 0;
    while (!stop) {
        int option = 0;
        system("CLS");
        printf("Command list:\n1.)View Current Objects\n2.)Edit An Object\n3.)Add A New Object\n4.)View An Object\n5.)Start\n6.)Start With A Predefined Position\n");
        scanf_s("%d", &option);
        switch (option) {
        case 1:
            viewObjects(toRet);
            break;
        case2:
            editObject(toRet);
            break;
        case 3:
            addObject(toRet);
            break;
        case 4:
            viewObject(toRet);
            break;
        case 5:
            if (toRet.first != NULL) {
                return toRet;
            }
            break;
        default:
            Obj* object = createObject(20, 500, 500, -0.5, -0.5, 500000, BLUE, true);
            Obj* object2 = createObject(20, 200, 200, 1, 0, 10000000, RED, false);
            objNode* node = createNode(object);
            objNode* node2 = createNode(object2);
            addObject(&toRet, node);
            addObject(&toRet, node2);
            return toRet;
            break;
        }
    }

}