#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

// rbtree 형의 주소값을 반환한다.
rbtree *new_rbtree(void) {
  // p는 rbtree형의 주소값을 저장하는 변수.
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // nilNode는 node_t형의 주소값을 저장하는 변수.
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));

  // nilNode의 색깔을 정의한다.
  nilNode -> color = RBTREE_BLACK;

  // 트리 p의 nil에 nilNode 할당. 센티넬 노드.
  p -> nil = nilNode;
  // 트리 p의 root에 nilNode 할당. 센티넬 노드.
  p -> root = nilNode;

  return p;
}

// 아무것도 반환하지 않는다.
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

// node_t 형의 주소값을 반환한다.
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  return t->root;
}

// node_t 형의 주소값을 반환한다.
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

// node_t 형의 주소값을 반환한다.
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

// node_t 형의 주소값을 반환한다.
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}