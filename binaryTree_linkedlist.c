typedef struct node *nodePointer;
/*노드를 나타내는 구조체이다*/
typedef struct node{
  int data;
  nodePointer leftChild, rightChild;  //왼쪽 자식 노드, 오른쪽 자식 노드를 가리키기 위한 포인터이다
}Node;

/*루트노드를 생성하는 함수이다*/
nodePointer setRoot(nodePointer a, int data){
  a = (nodePointer)malloc(sizeof(Node));  //구조체에 메모리 할당
  a->data = data;
  a->leftChild = NULL;  //초기 루트는 자식을 갖지 않으므로 자식 노드에 NULL값을 넣어준다
  a->rightChild = NULL;
  return a;
}

/*왼쪽 자식 노드를 생성하는 함수이다*/
nodePointer setLeftChild(nodePointer a, int data){
  if(a->leftChild){	//왼쪽 자식 노드가 비어있는지 확인한다
    printf("existing leftChild");
    exit(1);
  }
  nodePointer b = (nodePointer)malloc(sizeof(Node));	//새로운 자식 노드를 생성, 메모리를 할당한다.
  b->data = data;
  b->leftChild = NULL;  //새로 생긴 자식 노드들은 자식을 갖지 않으므로 NULL값을 넣어준다.
  b->rightChild = NULL;
  a->leftChild = b;  //부모 노드의 왼쪽 자식 포인터에 새로 만든 자식 노드포인터를 지정해준다.
  return b;
}

/*오른쪽 자식 노드를 생성하는 함수이다*/
nodePointer setRightChild(nodePointer a, int data){
  if(a->rightChild){
    printf("existing rightChild");
    exit(1);
  }
  nodePointer b = (nodePointer)malloc(sizeof(Node));
  b->data = data;
  b->leftChild = NULL;
  b->rightChild = NULL;
  a->rightChild = b;
  return b;
}

