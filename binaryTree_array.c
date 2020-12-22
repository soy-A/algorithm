/*노드를 나타내는 구조체이다*/
typedef struct {
    int data;
    short int flag;  //노드의 사용여부 체크
}Node;

/*노드들로 이루어진 배열과, 배열의 현재 크기를 포함하는 구조체이며, 구조체의 포인터타입을 선언해주었다*/
typedef struct tree *treePointer;
typedef struct tree{
    Node treeArray[MAX_SIZE];
};

/*트리 배열의 루트 노드에 값을 넣는 함수이다*/
void setRoot(treePointer a, int data){
    a->treeArray[1].data = data;
    a->treeArray[1].flag = TRUE;  //배열의 1번 인덱스는 사용되었음을 체크
}

/*트리 배열에 값을 넣는 함수이다*/
void setNode(treePointer a, int index, int data){
    if((a->treeArray[index/2].flag) != TRUE){	//삽입할 노드의 부모 노드가 존재하는지 체크
        printf("no existing parents node");
        exit(1);
    }
else if(a->treeArray[index].flag == TRUE){  //삽입할 위치에 이미 노드가 존재하는지 체크
        printf("existing node");
        exit(1);
    }
    a->treeArray[index].data = data;	//지정한 인덱스에 값 삽입
    a->treeArray[index].flag = TRUE;	//삽입한 노드임을 표시
}	

