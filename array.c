#include "array.h"

void append(Array* a, int e)
{
  if ((*a) == NULL)
  {
    (*a) = (Array) malloc(sizeof(Array));
    (*a)->t = (int*) malloc(sizeof(int)*1);
    (*a)->len = 1;
  }
  else
  {
    (*a)->len ++;
    (*a)->t = (int*) realloc((*a)->t,sizeof(int)*((*a)->len));
  }

  (*a)->t[(*a)->len-1] = e;
}

void display(Array a)
{
  if (a != NULL)
  {
    printf("Array :");

    for (int i=0; i<(a->len); i++)
    {
      printf(" %d",a->t[i]);
      if (i < a->len-1)
      {
        printf(",");
      }
    }

    printf(" : End \n\n");
  }
  else
  {
    printf("Array empty \n\n");
  }
}

void delete(Array* a)
{
  if (*a != NULL) {
    free((*a)->t);
    (*a)->len = 0;
    free((*a));
    (*a) = NULL;
  }
}

int getFromEnd(Array a, int i) {
  if (a != NULL && i < a->len )
  {
    return a->t[a->len-1-i];
  } else {
    printf("Array empty \n\n");
    return -1;
  }
}

int getLen(Array a) {
  if (a== NULL){
    return 0;
  } else {
    return a->len;
  }
}