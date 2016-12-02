#include <stdio.h>
#include <stdlib.h>

typedef
   struct _Node {
      struct _Node *l;
      struct _Node *r;
   } Node;

Node* mk(void)
{
   Node *x = malloc(sizeof(Node));
   x->l = NULL;
   x->r = NULL;
   return x;
}

Node* t;

void f(void)
{
   t       = mk();   // A
   t->l    = mk();   // B
   t->r    = mk();   // C
   t->l->l = mk();   // D
   t->l->r = mk();   // E
   t->r->l = mk();   // F
   t->r->r = mk();   // G

   free(t->l->l);
   t->l->l = NULL;
   free(t->r->l);
   t->r->l = NULL;
   free(t->r->r);
   t->r->r = NULL;
   free(t->r);
   t->r = NULL;
}

int main(void)
{
   f();
   return 0;
}

