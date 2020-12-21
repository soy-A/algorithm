#include <iostream>
#define M 3 // 차수
using namespace std;

class Node {
  int *key; // 키를 저장
  int count;  // 몇 개의 키가 들어있는지 카운트
  Node **ptr;
  bool leaf;  // 리프노드인지를 표시

public:
  Node(bool leaf1); // Node의 생성자

  void printTree();  // 트리 출력 함수

  void insertNotFull(int k);
  void splitChild(int i, Node *y);
  void deleteKey(int k);
  void removeFromLeaf(int index);
  void removeFromNonLeaf(int index);
  int getLeftRight(int index);
  int getRightLeft(int index);
  void fill(int index);
  void borrowFromLeft(int index);
  void borrowFromRight(int index);
  void merge(int index);
  friend class Btree;
};

class Btree {
  Node *root; // 루트 노드

public:
  Btree() {
    root = NULL;  // 루트 노드 초기화
  }

  void printTree() {
    if (root != NULL)
      root->printTree();
  }

  void insertKey(int k);
  void deleteKey(int k);
};

Node::Node(bool leaf1) {  // 노드 생성자
  leaf = leaf1;

  key = new int[2 * M - 1];
  ptr = new Node *[2 * M];

  count = 0;  // 카운트 초기화
}

void Node::deleteKey(int k) { // key를 지우는 함수
  int index = 0;
  while (index < count && key[index] < k){// 노드가 가진 키 중 k의 인덱스를 찾아 저장
    ++index;
  }
  if (index < count && key[index] == k) { // 현재 노드에 일치하는 키가 존재하면 삭제
    if(leaf) {
      removeFromLeaf(index);
    } else {
      removeFromNonLeaf(index);
    }
  } else{
    if (leaf) {
        cout << "\n키가 존재하지 않습니다.\n";
        return;
      }

    bool flag = ((index == count) ? true : false);

    if(ptr[index]->count < M) { // 자식노드의 키 개수가 삭제하기에 모자르면 채워놓는다
      fill(index);
    }

    if(flag && index > count) { // 자식노드 합쳐졌다면 현재 노드의 키가 내려가고 한 칸씩 당겨졌으므로 index-1의 자식노드에서 키를 삭제
      ptr[index - 1]->deleteKey(k);
    } else {
      ptr[index]->deleteKey(k);
    }
  }
}

void Node::removeFromLeaf(int index) {    // 리프노드에서 삭제
  for(int i = index + 1; i < count; ++i){
    key[i - 1] = key[i];
  }
  count--;
}

void Node::removeFromNonLeaf(int index) { // 자식 가진 노드에서 삭제
  int k = key[index];

  if(ptr[index]->count >= M) {  // 왼쪽 자식 노드의 키 개수가 충분하면 왼쪽자식의 가장 큰 값 삭제
    int left = getLeftRight(index);
    key[index] = left;
    ptr[index]->deleteKey(left);
  }

  else if(ptr[index + 1]->count >= M) { // 오른쪽 자식 노드의 키 개수가 충분하면 오른쪽 자식의 가장 작은 값 삭제
    int right = getRightLeft(index);
    key[index] = right;
    ptr[index + 1]->deleteKey(right);
  }

  else {  // 둘 다 부족하면 병합
    merge(index);
    ptr[index]->deleteKey(k);
  }
}

int Node::getLeftRight(int index) { // 왼쪽 자식노드의 자식노드들 중 가장 오른쪽 값
  Node *cur = ptr[index];
  while(!cur->leaf)
    cur = cur->ptr[cur->count];

  return cur->key[cur->count - 1];
}

int Node::getRightLeft(int index) { // 오른쪽 자식노드의 자식노드들 중 가장 왼쪽 값
  Node *cur = ptr[index + 1];
  while(!cur->leaf)
    cur = cur->ptr[0];

  return cur->key[0];
}

void Node::fill(int index) {  // 최소 키 개수를 지키기 위해 빌리거나 합치도록 하는 함수
  if(index != 0 && ptr[index - 1]->count >= M) // 왼쪽 자식의 키 개수가 최소개수보다 많을 때 왼쪽자식을 빌린다
    borrowFromLeft(index);

  else if(index != count && ptr[index + 1]->count >= M)  // 오른쪽 자식의 키 개수가 최소개수보다 많을 때 오른쪽자식을 빌린다
    borrowFromRight(index);

  else {  // 빌릴 수 없다면 합친다
    if(index != count)
      merge(index);
    else
      merge(index - 1); // 가장 오른쪽 자식노드는 왼쪽 형제와 합친다.
  }

}

void Node::borrowFromLeft(int index) {  // 왼쪽자식에서 빌리는 함수
  Node *child = ptr[index];
  Node *sibling = ptr[index - 1];

  for(int i = child->count - 1; i >= 0; --i){ // 오른쪽으로 한 칸씩 이동
    child->key[i + 1] = child->key[i];
  }

  if(!child->leaf) {  // 자식노드를 가질 때 자식노드들도 한 칸씩 이동
    for (int i = child->count; i >= 0; --i)
      child->ptr[i + 1] = child->ptr[i];
  }

  child->key[0] = key[index - 1]; // 비워놓은 자식노드 처음 값에 키값을 넣는다

  if(!child->leaf){
    child->ptr[0] = sibling->ptr[sibling->count];
  }
  key[index - 1] = sibling->key[sibling->count - 1];

  child->count += 1;
  sibling->count -= 1;

  return;
}

void Node::borrowFromRight(int index) {  // 오른쪽 자식에서 빌리는 함수
  Node *child = ptr[index];
  Node *sibling = ptr[index + 1];

  child->key[(child->count)] = key[index];

  if(!(child->leaf)){
    child->ptr[(child->count) + 1] = sibling->ptr[0];
  }
  key[index] = sibling->key[0];

  for(int i = 1; i < sibling->count; ++i){
    sibling->key[i - 1] = sibling->key[i];
  }
  if(!sibling->leaf) {
    for(int i = 1; i <= sibling->count; ++i)
      sibling->ptr[i - 1] = sibling->ptr[i];
  }

  child->count += 1;
  sibling->count -= 1;

  return;
}

void Node::merge(int index) { // 노드를 합치는 함수
  Node *child = ptr[index]; // 자식노드
  Node *sibling = ptr[index + 1]; // 자식노드의 오른쪽 형제

  child->key[M - 1] = key[index]; // 자식노드의 중간에 기준 노드의 키를 입력

  for(int i = 0; i < sibling->count; ++i) // 형제노드의 키값들을 전부 자식노드에 옮겨 붙여준다
    child->key[M + i] = sibling->key[i];

  if(!child->leaf) {  // 리프노드가 아니면 형제노드의 노드 포인터들도 옮겨 붙여준다
    for(int i = 0; i <= sibling->count; ++i)
      child->ptr[M + i] = sibling->ptr[i];
  }

  for(int i = index + 1; i < count; ++i)  // 기준 노드부터 한 칸씩 당긴다
    key[i - 1] = key[i];

  for(int i = index + 2; i <= count; ++i) // 기준 노드의 노드 포인터도 한 칸씩 당겨준다
    ptr[i - 1] = ptr[i];

  child->count += sibling->count + 1; // 왼쪽 자식 노드의 카운트에 가져온 형제 자식 노드의 카운트와 1(기준 노드의 키 가져옴) 더한다
  count--;  // 기준 노드의 키 하나 없어졌으므로 빼준다

  delete (sibling); // 다 옮겨주었으므로 삭제
  return;
}

void Btree::insertKey(int k) {  // 키 삽입 함수
  if (root == NULL) {
    root = new Node(true);
    root->key[0] = k;
    root->count = 1;
  } else {
    if (root->count == 2 * M - 1) { // 가득 차있는 경우
      Node *tmp = new Node(false);

      tmp->ptr[0] = root;
      tmp->splitChild(0, root);

      int i = 0;
      if (tmp->key[0] < k){
        i++;
      }
      tmp->ptr[i]->insertNotFull(k);
      root = tmp;
    } else
      root->insertNotFull(k);
  }
}

void Node::insertNotFull(int k) { // 노드가 충분할 때 삽입
  int i = count - 1;

  if (leaf == true) {
    while (i >= 0 && key[i] > k) {
      key[i + 1] = key[i];
      i--;
    }

    key[i + 1] = k;
    count = count + 1;
  } else {
    while (i >= 0 && key[i] > k)
      i--;

    if (ptr[i + 1]->count == 2 * M - 1) {
      splitChild(i + 1, ptr[i + 1]);

      if (key[i + 1] < k)
        i++;
    }
    ptr[i + 1]->insertNotFull(k);
  }
}

void Node::splitChild(int i, Node *y) { // 자식 나누는 함수
  Node *z = new Node(y->leaf);
  z->count = M - 1;

  for (int j = 0; j < M - 1; j++)
    z->key[j] = y->key[j + M];

  if (y->leaf == false) {
    for (int j = 0; j < M; j++)
      z->ptr[j] = y->ptr[j + M];
  }

  y->count = M - 1;

  for (int j = count; j >= i + 1; j--)
    ptr[j + 1] = ptr[j];

  ptr[i + 1] = z;

  for (int j = count - 1; j >= i; j--)
    key[j + 1] = key[j];

  key[i] = y->key[M - 1];

  count = count + 1;
}

void Node::printTree() {  // 트리 출력
  int i;
  for (i = 0; i < count; i++) {
    if (leaf == false){
      ptr[i]->printTree();
    }
    cout << " " << key[i];
  }

  if (leaf == false){
    ptr[i]->printTree();
  }
  cout << endl;
}

void Btree::deleteKey(int k) {  // 키 삭제 함수
  root->deleteKey(k);

  if (root->count == 0) {
    Node *tmp = root;
    if (root->leaf)
      root = NULL;
    else
      root = root->ptr[0];

    delete tmp;
  }
  return;
}

int main() {

  int selected, key;
  Btree t;

  while(1){
    cout << "원하는 작업을 입력하세요(1.삽입 2.삭제): ";
    cin >> selected;
    if(selected == 1){
      while(1){
        cout << "삽입할 키 값을 입력하세요(작업 선택은 -1): ";
        cin >> key;
        if(key != -1){
          t.insertKey(key);
          cout << "-----------------------------" << endl;
          t.printTree();
          cout << "\n-----------------------------" << endl;
        } else {
          break;
        }
      }
    } else if(selected == 2){
      while(1){
        cout << "삭제할 키 값을 입력하세요(작업 선택은 -1): ";
        cin >> key;
        if(key != -1){
          t.deleteKey(key);
          cout << "-----------------------------" << endl;
          t.printTree();
          cout << "\n-----------------------------" << endl;
        } else {
          break;
        }
      }
    }
  }
}

