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
  // node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));

  // 트리 p의 nil에 nilNode 할당. 센티넬 노드.
  p -> nil = NULL;
  // 트리 p의 root에 nilNode 할당. 센티넬 노드.
  p -> root = NULL;

  return p;
}

/**
 * RB tree를 삭제한다.
 * @param t *t 삭제할 트리의 주소값
 * @return 아무것도 반환하지 않는다.
*/
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

/**
 * 이진 탐색 트리의 삽입 규칙을 지키면서 insert
 * @param n *n 해당 노드의 주소값
 * @param key 새로운 노드의 key
 * @return
*/
node_t *path_find(node_t *n, key_t key) {
  int flag = (key <= n -> key) ? 0 : 1;

  if (flag) {
    // 새로운 노드의 키값이 현재 노드의 키값보다 크면? 오른쪽으로 들어감.
    // 해당 노드의 오른쪽 노드가 비어있다면 현재 노드의 주소값 반환
    // 비어있지 않다면 오른쪽으로 들어감.

    return n -> right == NULL ? n : path_find(n -> right, key);
  }else {
    // 새로운 노드의 키값이 현재 노드의 키값보다 작으면? 왼쪽으로 들어감.
    // 해당 노드의 왼쪽 노드가 비어있다면 현재 노드의 주소값 반환
    // 비어있지 않다면 왼쪽으로 들어감.

    return n -> left == NULL ? n : path_find(n -> left, key);
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
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node -> color = RBTREE_RED;
  new_node -> key = key;

  if(t -> root == NULL) {
    // 루트 노드가 nil 노드일 경우 
    new_node -> color = RBTREE_BLACK;
    t -> root = new_node;
  }else {
    // 루트 노드가 nil 노드가 아닌 경우 즉, new_node가 첫 노드가 아닌 경우
    // 이진탐색트리의 규칙을 지키면서 삽입.
    // 트리의 키 값을 비교해 가면서 재귀적으로 final node를 찾는다.
    node_t *final_node;

    final_node = path_find(t -> root, key);

    if (key <= final_node -> key) {
      final_node -> left = new_node;
    }else {
      final_node -> right = new_node;
    }
    new_node -> parent = final_node;

    // 방금 삽입된 노드의 부모 노드가 red일 때 -> red는 중복으로 들어올 수 없다는 규칙 위반.
    if(final_node -> color == RBTREE_RED) {
      node_t *my_parent = new_node -> parent;
      node_t *my_grand_parent = my_parent -> parent;
      node_t *my_grand_grand_parent = my_grand_parent -> parent;
      node_t *my_uncle;

      // 삼촌이 레드일 경우 1 , 블랙이거나 없을경우 0
      int is_alive = 0;

      if(new_node -> key > my_grand_parent -> key) {
        my_uncle = my_grand_parent -> left ? my_grand_parent -> left : NULL;
        is_alive = my_uncle ? my_uncle -> color == RBTREE_RED ? 1 : 0 : 0;
      }else {
        my_uncle = my_grand_parent -> right ? my_grand_parent -> right : NULL;
        is_alive = my_uncle ? my_uncle -> color == RBTREE_RED ? 1 : 0 : 0;
      }

      if(is_alive) {
        // 삼촌이 레드일 경우 - Recoloring
        // 삽입된 노드의 부모와 삼촌 노드를 검은색으로, 부모의 부모 노드를 빨간색으로.
        my_parent -> color = RBTREE_BLACK;
        my_uncle -> color = RBTREE_BLACK;
        my_grand_parent -> color = RBTREE_RED;

        if (t -> root -> color == 0) {
          t -> root -> color = RBTREE_BLACK;
        }

      }else {
        // 삼촌이 블랙이거나 안계심 - Restructuring
        // 삽입된 노드, 부모 노드, 부모의 부모 노드를 오름차순으로 정렬.
        // new_node, my_parent, my_grand_parent
        // 그 중 중간 값을 부모 노드로 만들고 나머지 노드들을 자식으로 변환한다.
        // 부모 노드가 된 노드를 검은색으로 나머지를 빨간색으로.
        int arr[3];

        arr[0] = new_node -> key;
        arr[1] = my_parent -> key;
        arr[2] = my_grand_parent -> key;

        for(int i=0; i < 3; i++) {
            for(int j=0; j < 3 - i - 1; j++) {
                if(arr[j] > arr[j+1]){    // swap
                    int temp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }        

        node_t *max, *mid, *min;

        for(int i = 0; i < 3; i++) {
          if(i == 0) {
            if (arr[i] == new_node -> key) {
              min = new_node;
            }

            if(arr[i] == my_parent -> key) {
              min = my_parent;
            }

            if(arr[i] == my_grand_parent -> key) {
              min = my_grand_parent;
            }
          }

          if(i == 1) {
            if(arr[i] == new_node -> key) {
              mid = new_node;
            }

            if(arr[i] == my_parent -> key) {
              mid = my_parent;
            }

            if(arr[i] == my_grand_parent -> key) {
              mid = my_grand_parent;
            }
          }

          if(i == 2) {
            if(arr[i] == new_node -> key) {
              max = new_node;
            }

            if(arr[i] == my_parent -> key) {
              max = my_parent;
            }

            if(arr[i] == my_grand_parent -> key) {
              max = my_grand_parent;
            }
          }
        }

        // mid를 부모 노드로 만든다. 그리고 나머지를 자식 노드로 만든다.
        mid -> parent = NULL;
        mid -> left = NULL;
        mid -> right = NULL;
        
        max -> parent = NULL;
        max -> left = NULL;
        max -> right = NULL;

        min -> parent = NULL;
        min -> left = NULL;
        min -> right = NULL;

        mid -> left = min;
        mid -> right = max;
        min -> parent = mid;
        max -> parent = mid;

        // 부모 노드가 된 노드를 검은색 그리고 나머지 빨간색.
        mid -> color = RBTREE_BLACK;
        min -> color = RBTREE_RED;
        max -> color = RBTREE_RED;

        // 증조 할아버지 바인딩.
        if(my_grand_grand_parent) {
          if(my_grand_grand_parent -> key <= mid -> key) {
            my_grand_grand_parent -> right = NULL;
            my_grand_grand_parent -> right = mid;
          }else {
            my_grand_grand_parent -> left = NULL;
            my_grand_grand_parent -> left = mid;
          }
        }else {
          t -> root = mid;
        }
      }
    }
  };

  return new_node;
}

node_t *node_find(node_t *n, key_t key) {
  // 깊이 판단 left, rigth NULL

  int flag = n -> key > key ? 1 : 0;

  if(n -> key == key) {
    return n;
  }else {
    if (flag) {
      return n -> right == NULL ? NULL : node_find(n -> right, key);
    }else {
      return n -> left == NULL ? NULL : node_find(n -> left, key);
    }
  }
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // key를 가지고 재귀적으로 찾아나감
  node_t *n = node_find(t->root, key);

  return n;
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

// int main(){
//   rbtree *t = new_rbtree();
//   node_t *p = rbtree_insert(t, 20);

//   node_t *q = rbtree_find(t, 20);

//   return 0;
// }