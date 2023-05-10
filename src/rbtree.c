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
 * @param t 삭제할 트리의 주소값
*/
void delete_rbtree(rbtree *t) {
  // 후위 순회하면서 메모리를 반환한다.
  rbtree_delete_helper(t -> nil, t->root);

  free(t);
}

/**
 * x를 기준으로 left rotation을 수행한다.
 * @param tree 트리
 * @param x 기준 노드
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

/**
 * x를 기준으로 right rotation을 수행한다.
 * @param tree 트리
 * @param x 기준 노드
 * 
*/
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
 * 삽입 연산 과정 중 RBtree의 속성을 위배했을 경우 fix 수행
 * @param t 트리
 * @param z 새로 삽입 할 노드
*/
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t *y;
  
  while (z -> parent -> color == RBTREE_RED) {
    if (z -> parent == z -> parent -> parent -> left) {
      // z의 부모노드가 할아버지 노드의 왼쪽 노드일 경우
      y = z -> parent -> parent -> right;

      if (y -> color == RBTREE_RED) {
        // z의 삼촌 y가 RED
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      }else {
        // z의 삼촌 y가 BLACK 
        if (z == z -> parent -> right) {
          // 삽입한 노드가 오른쪽 자식일 때
          z = z -> parent;
          left_rotate(t, z);
        }

        // 삽입한 노드가 왼쪽 자식일 때
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, z -> parent -> parent);
      }
    }else {
      // z의 부모노드가 할아버지 노드의 오른쪽 노드일 경우
      y = z -> parent -> parent -> left;

      if (y -> color == RBTREE_RED) {
        // z의 삼촌 y가 RED
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      }else {

        if (z == z -> parent -> left) {
          // 삽입한 노드가 오른쪽 자식일 때
          z = z -> parent;
          right_rotate(t, z);
        }

        // 삽입한 노드가 왼쪽 자식일 때
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        left_rotate(t, z -> parent -> parent);
      }
    }
  }

  // 루트는 항상 검은색.
  t -> root -> color = RBTREE_BLACK;
}

/**
 * 이진 탐색 트리의 삽입 규칙을 지키면서 insert
 * @param n *n 해당 노드의 주소값
 * @param key 새로운 노드의 key
 * @return 마지막 노드의 주소값
*/
node_t *path_find(rbtree *t, node_t *n, key_t key) {
  int flag = (key <= n -> key) ? 0 : 1;

  if (flag) {
    // 새로운 노드의 키값이 현재 노드의 키값보다 크면? 오른쪽으로 들어감.
    // 해당 노드의 오른쪽 노드가 비어있다면 현재 노드의 주소값 반환
    // 비어있지 않다면 오른쪽으로 들어감.

    return n -> right == t -> nil  ? n : path_find(t, n -> right, key);
  }else {
    // 새로운 노드의 키값이 현재 노드의 키값보다 작으면? 왼쪽으로 들어감.
    // 해당 노드의 왼쪽 노드가 비어있다면 현재 노드의 주소값 반환
    // 비어있지 않다면 왼쪽으로 들어감.

    return n -> left == t -> nil ? n : path_find(t, n -> left, key);
  }
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

  // 새로 추가할 노드의 메모리 할당.
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  
  // 새로 추가할 노드의 key 바인딩.
  z -> key = key;

  // 이진탐색트리의 삽입 연산 수행
  while (x != t -> nil) {
    y = x;
    if (z -> key < x -> key)
      x = x -> left;
    else
      x = x -> right; 
  }

  z -> parent = y;

  printf("%d ", y == t->nil ? 1 : y->key);

  if (y == t -> nil) {
    t -> root = z;
  }else if(z -> key < y -> key) {
    y -> left = z;
  }else {
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
        // x의 형제 w가 red
        // x의 형제 w를 black으로 바꾼다 (부모와 형제의 색 교환 -> 부모 기준 왼쪽으로 회전.) 
        // case 중 하나로 해결 (if문 이후에서 case 판별)
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        left_rotate(t, x -> parent);
        w = x -> parent -> right;
      }

      if(w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK) {
        // x의 형제 w의 왼쪽 자녀가 black & 오른쪽 자녀가 black
        // doubly black과 w를 모아서 부모에게 전달하여 처리 위임
        w -> color = RBTREE_RED;
        x = x -> parent;
      }else { 
        if (w -> right -> color == RBTREE_BLACK) {
          // x의 형제 w의 왼쪽 자녀가 red & 오른쪽 자녀가 black
          w -> left -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          right_rotate(t, w);
          w = x -> parent -> right;
        }

        // x의 형제 w의 오른쪽 자녀가 red
        // doubly black의 부모를 red로 변경 후 extra black 전달 
        // 후에 black으로 변경
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> right -> color = RBTREE_BLACK;
        left_rotate(t, x -> parent);
        x = t -> root;
      }
    }
    else {
      // 왼쪽 형제 기준 즉, 반대 경우
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
  // 삭제되는 색 저장
  // 삭제되는 색 1. 자녀가 없을떄: 삭제되는 노드의 색. 2. 자녀가 하나일 때: 삭제되는 노드의 색. 3. 자녀가 둘일때: successor의 색.
  org_color = y -> color;

  // 이진탐색트리의 삭제 연산을 진행.
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

  // 삭제되는 색이 black일 때 속성 위반(임의의 노드에서 말단 노드까지 가는 경로에 포함된 black 노드의 수는 같다.)
  // fixup 진행
  if (org_color == RBTREE_BLACK) {
    delete_fixup(t, x);
  }

  free(p);

  return 0;
}



int main() {

  rbtree *t = new_rbtree();

  rbtree_insert(t, 10);
  rbtree_insert(t, 98);
  rbtree_insert(t, 44);

}