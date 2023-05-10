#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * 새로운 RB tree를 만든다.
 * @return p rbtree 형의 주소값을 반환한다.
*/
rbtree *new_rbtree(void) {
  // p는 rbtree형의 주소값을 저장하는 변수.
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // nilNode는 node_t형의 주소값을 저장하는 변수.
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
  nil_node -> color = RBTREE_BLACK;

  // 트리 p의 nil에 nilNode 할당. 센티넬 노드.
  p -> nil = nil_node;
  // 트리 p의 root에 nilNode 할당. 센티넬 노드.
  p -> root = nil_node;

  return p;
}

/**
 * RB tree를 후위 순회함
 * @param nil nil 노드 주소 
 * @param s 순회를 시작할 노드
*/
void rbtree_delete_helper(node_t *nil, node_t *s) {
  if(s == nil) {
    return;
  }

  if(s -> left != nil) {
    rbtree_delete_helper(nil, s->left);
  }

  if(s -> right != nil) {
    rbtree_delete_helper(nil, s->right);
  }

  printf("%d", s -> key);
}

/**
 * RB tree를 삭제한다.
 * @param t *t 삭제할 트리의 주소값
 * @return 아무것도 반환하지 않는다.
*/
void delete_rbtree(rbtree *t) {
  // 트리를 후위 순회하면서 메모리를 반환한다.
  rbtree_delete_helper(t -> nil, t->root);

  free(t);
}


/**
 * 
 * 
*/
void left_rotate(rbtree *tree, node_t *x) {
  node_t *y;

  y = x -> right;
  x -> right = y -> left;
  
  if (y -> left != tree -> nil) 
      y -> left -> parent = x;
  
  y -> parent = x -> parent;
  
  if (x -> parent == tree -> nil) 
      tree -> root = y;
  else if (x == x -> parent -> left)
      x -> parent -> left = y;
  else 
      x -> parent -> right = y;

  y -> left = x;
  x -> parent = y;
}

void right_rotate(rbtree *tree, node_t *x) {
  node_t *y;

  y = x -> left;
  x -> left = y -> right;

  if(y -> right != tree -> nil) {
    y -> right -> parent = x;
  }

  y -> parent = x -> parent;

  if (x -> parent == tree -> nil){
    tree -> root = y;
  }else if(x == x -> parent -> left) {
    x -> parent -> left = y;
  }else {
    x -> parent -> right = y;
  }

  y -> right = x;
  x -> parent = y;
}

/**
 * 
 * 
 * 
*/
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t *y;
  
  while (z -> parent -> color == RBTREE_RED) {
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    if (z -> parent == z -> parent -> parent -> left) {
      y = z -> parent -> parent -> right;
      
      // CASE 1 : 노드 z의 삼촌 y가 적색인 경우
      if (y -> color == RBTREE_RED) {
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      }
      // CASE 2 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z -> parent -> right) {
          z = z -> parent;
          left_rotate(t, z);
        }
        // CASE 3 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, z -> parent -> parent);
      }
    }
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    else {
      y = z -> parent -> parent -> left;

      // CASE 4 : 노드 z의 삼촌 y가 적색인 경우
      if (y -> color == RBTREE_RED) {
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      }
      // CASE 5 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z -> parent -> left) {
          z = z -> parent;
          right_rotate(t, z);
        }
        // CASE 6 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        left_rotate(t, z -> parent -> parent);
      }
    }
  }
  t -> root -> color = RBTREE_BLACK;
}

/**
 * RB tree에 노드를 추가한다.
 * @param t 
 * @param key
 * @return
 *  
*/
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y = t -> nil;
  node_t *x = t -> root;
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  
  z -> key = key;

  while (x != t -> nil) {
    y = x;
    if (z -> key < x -> key)
      x = x -> left;
    else
      x = x -> right; 
  }

  z -> parent = y;
  
  if (y == t -> nil) {
    t -> root = z;
  }
  else if (z -> key < y -> key) {
    y -> left = z;
  }
  else {
    y -> right = z;
  }

  z -> left = t -> nil;
  z -> right = t -> nil;
  z -> color = RBTREE_RED;

  rbtree_insert_fixup(t, z);

  return z;
}

/**
 * RB tree에서 노드를 찾음
 * @param t 탐색할 트리의 주소값
 * @param key 찾을 노드의 키값
*/
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // key를 가지고 재귀적으로 찾아나감
  node_t *n = t -> root;

  while (n != t -> nil) {
    if(n->key < key) {
      n = n-> right;
    }else if(n -> key > key) {
      n = n -> left;
    }else {
      return n;
    }
  }

  return NULL;
}

/**
 * RB tree에서 최솟값을 가진 노드를 찾는다.
 * @param t 탐색할 트리의 주소값
*/
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *n = t -> root;
  node_t *temp;

  while (n -> left != t -> nil) {
    temp = n -> left;

    n = temp;
  }

  return n;
}

/**
 * RB tree에서 최솟값을 가진 노드를 찾는다.
 * @param t 탐색할 트리의 주소값
*/
node_t *rbtree_sub_min(rbtree *t, node_t *p) {
  // TODO: implement find
  node_t *n = p;
  node_t *temp;

  while (n -> left != t -> nil) {
    temp = n -> left;

    n = temp;
  }

  return n;
}

/**
 * RB tree에서 최대값을 가진 노드를 찾는다.
 * @param t 탐색할 트리의 주소값
*/
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *n = t -> root;
  node_t *temp;

  while(n -> right != t -> nil) {
    temp = n -> right;

    n = temp;
  }

  return n;
}

/**
 * RB tree를 중위 순회함
 * @param s 현재 순회 중인 노드
 * @param arr 배열 첫 원소의 주소값
 * @param idx 현재 배열의 크기
 * @param n 배열의 크기
*/
void rbtree_to_array_helper(node_t *nil, node_t *s, key_t *arr, size_t *idx, size_t n) {
  if(*idx >= n) {
    return;
  }

  if(s -> left != nil) {
    rbtree_to_array_helper(nil, s->left, arr, idx, n);
  }

  arr[(*idx)++] = s -> key;

  if(s -> right != nil) {
    rbtree_to_array_helper(nil, s->right, arr, idx, n);
  }
}

/**
 * RB tree를 중위 순회한 결과를 배열로 저장함
 * @param t 순회할 트리
 * @param arr 배열 첫 원소의 주소값
 * @param n 배열의 크기
*/
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  size_t idx = 0;
  rbtree_to_array_helper(t -> nil, t -> root, arr, &idx, n);
  return idx;
}

/**
 * 트리 내부 이동
 * @param t 트리
 * @param u 서브트리의 루트
 * @param v 서브트리의 루트
*/
void trans_plant(rbtree *t, node_t *u, node_t *v) {
  if (u -> parent == t -> nil) {
    t -> root = v;
  }
  else if (u == u -> parent -> left) {
    u -> parent -> left = v;
  }
  else {
    u -> parent -> right = v;
  }
  
  v -> parent = u -> parent;
}

/**
 * 
 * 
*/
void delete_fixup(rbtree *t, node_t *x) {
  while (x != t -> root && x -> color == RBTREE_BLACK) {
    if (x == x -> parent -> left){
      node_t *w = x -> parent -> right;
      
      if(w -> color == RBTREE_RED) {
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        left_rotate(t, x -> parent);
        w = x -> parent -> right;
      }

      if(w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK) {
        w -> color = RBTREE_RED;
        x = x -> parent;
      }else { 
        
        if (w -> right -> color == RBTREE_BLACK) {
          w -> left -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          right_rotate(t, w);
          w = x -> parent -> right;
        }

        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> right -> color = RBTREE_BLACK;
        left_rotate(t, x -> parent);
        x = t -> root;
      }
    }
    else {
      node_t *w = x -> parent -> left;

      if (w -> color == RBTREE_RED) {
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        right_rotate(t, x -> parent);
        w = x -> parent -> left;
      }

      if (w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK) {
        w -> color = RBTREE_RED;
        x = x -> parent;
      }else {

        if (w -> left -> color == RBTREE_BLACK) {
          w -> right -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          left_rotate(t, w);
          w = x -> parent -> left;
        }

        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> left -> color = RBTREE_BLACK;
        right_rotate(t, x -> parent);
        x = t->root;
      }
    }
  }

  x ->color = RBTREE_BLACK;
}

/**
 * RB tree에서 특정 노드를 삭제
 * @param t 노드를 삭제할 트리
 * @param z 삭제할 노드의 주소값
*/
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y;
  node_t *x;
  color_t org_color;

  y = p;
  org_color = y -> color;

  if (p -> left == t -> nil) {
    x = p -> right;
    trans_plant(t, p, p -> right);
  }
  else if (p -> right == t -> nil) {
    x = p -> left;
    trans_plant(t, p, p -> left);
  }
  else {
    y = p -> right;
    while(y -> left != t -> nil){
      y = y -> left;
    }
    org_color = y -> color;
    x = y -> right;

    if (y -> parent == p) {
      x -> parent = y;
    } 
    else {
      trans_plant(t, y, y -> right);
      y -> right = p -> right;
      y -> right -> parent = y;
    }

    trans_plant(t, p, y);
    y -> left = p -> left;
    y -> left -> parent = y;
    y -> color = p -> color;
  }

  if (org_color == RBTREE_BLACK) {
    delete_fixup(t, x);
  }

  free(p);

  return 0;
}