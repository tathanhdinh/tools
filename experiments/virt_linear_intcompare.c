/* Generated by CIL v. 1.7.0 */
/* print_CIL_Input is false */
#include <stdlib.h>

int compare(int x , int y ) ;

struct vcmd$keyValueStruct
{
  unsigned char vcmd$key ;
  void *vcmd$value ;
};

enum vcmd$op
  {
    vcmd_formal$result_STA_0$value_LIT_0                    = 192,
    vcmd_Gt_int_int2int$right_STA_0$result_STA_0$left_STA_1 = 2,
    vcmd_constant_int$result_STA_0$value_LIT_0              = 253,
    vcmd_goto$label_LAB_0                                   = 149,
    vcmd_Lt_int_int2int$left_STA_0$result_STA_0$right_STA_1 = 165,
    vcmd_label$label_LAB_0                                  = 114,
    vcmd_return_int$expr_STA_0                              = 127,
    vcmd_load_int$left_STA_0$result_STA_0                   = 82,
    vcmd_branchIfTrue$expr_STA_0$label_LAB_0                = 184
  };

unsigned char vcmd$array[1][70]  =
  {
    {
      vcmd_formal$result_STA_0$value_LIT_0,        0,        0,        0,
      0,        vcmd_load_int$left_STA_0$result_STA_0,        vcmd_formal$result_STA_0$value_LIT_0,        1,
      0,        0,        0,        vcmd_load_int$left_STA_0$result_STA_0,
      vcmd_Gt_int_int2int$right_STA_0$result_STA_0$left_STA_1,        vcmd_branchIfTrue$expr_STA_0$label_LAB_0,        9,        0,
      0,        0,        vcmd_goto$label_LAB_0,        10,
      0,        0,        0,        vcmd_constant_int$result_STA_0$value_LIT_0,
      1,        0,        0,        0,
      vcmd_return_int$expr_STA_0,        vcmd_formal$result_STA_0$value_LIT_0,        1,        0,
      0,        0,        vcmd_load_int$left_STA_0$result_STA_0,        vcmd_formal$result_STA_0$value_LIT_0,
      0,        0,        0,        0,
      vcmd_load_int$left_STA_0$result_STA_0,        vcmd_Lt_int_int2int$left_STA_0$result_STA_0$right_STA_1,        vcmd_branchIfTrue$expr_STA_0$label_LAB_0,        9,
      0,        0,        0,        vcmd_goto$label_LAB_0,
      10,        0,        0,        0,
      vcmd_constant_int$result_STA_0$value_LIT_0,        255,        255,        255,
      255,        vcmd_return_int$expr_STA_0,        vcmd_constant_int$result_STA_0$value_LIT_0,        0,
      0,        0,        0,        vcmd_return_int$expr_STA_0,
      vcmd_constant_int$result_STA_0$value_LIT_0,        0,        0,        0,
      0,        vcmd_return_int$expr_STA_0
    }
  };


union vcmd$node {
   void *_void_star ;
   int _int ;
};

int compare(int x , int y )
{
  union vcmd$node vstack[1][32] ;
  union vcmd$node *vsp[1] ;
  unsigned char *vpc[1] ;

  const static struct vcmd$keyValueStruct vcmd$jumpTab[9]  =
    {
      {
        vcmd_Gt_int_int2int$right_STA_0$result_STA_0$left_STA_1,
        && vcmd_Gt_int_int2int$right_STA_0$result_STA_0$left_STA_1
      },
      {
        vcmd_load_int$left_STA_0$result_STA_0,
        && vcmd_load_int$left_STA_0$result_STA_0
      },
      {
        vcmd_label$label_LAB_0,
        && vcmd_label$label_LAB_0
      },
      {
        vcmd_return_int$expr_STA_0,
        && vcmd_return_int$expr_STA_0
      },
      {
        vcmd_goto$label_LAB_0,
        && vcmd_goto$label_LAB_0
      },
      {
        vcmd_Lt_int_int2int$left_STA_0$result_STA_0$right_STA_1,
        && vcmd_Lt_int_int2int$left_STA_0$result_STA_0$right_STA_1
      },
      {
        vcmd_branchIfTrue$expr_STA_0$label_LAB_0,
        && vcmd_branchIfTrue$expr_STA_0$label_LAB_0
      },
      {
        vcmd_formal$result_STA_0$value_LIT_0,
        && vcmd_formal$result_STA_0$value_LIT_0
      },
      {
        vcmd_constant_int$result_STA_0$value_LIT_0,
        && vcmd_constant_int$result_STA_0$value_LIT_0
      }
    };

  unsigned char vcmd$currentOp;
  int vcmd$searchVar ;

  vsp[0] = vstack[0];
  vpc[0] = vcmd$array[0];
 _topOfLoop: /* CIL Label */ ;
  vcmd$currentOp = *(vpc[0]);
  vcmd$searchVar = 0;
  while (vcmd$jumpTab[vcmd$searchVar].vcmd$key != vcmd$currentOp) {
    vcmd$searchVar ++;
  }
  goto *(vcmd$jumpTab[vcmd$searchVar].vcmd$value);

 vcmd_goto$label_LAB_0: /* CIL Label */ ;
  (vpc[0]) ++;
  vpc[0] += *((int *)vpc[0]);
  goto _topOfLoop;

 vcmd_Gt_int_int2int$right_STA_0$result_STA_0$left_STA_1: /* CIL Label */ ;
  (vpc[0]) ++;
  (vsp[0] + -1)->_int = (vsp[0] + -1)->_int > (vsp[0] + 0)->_int;
  (vsp[0]) --;
  goto _topOfLoop;

 vcmd_branchIfTrue$expr_STA_0$label_LAB_0: /* CIL Label */ ;
  (vpc[0]) ++;
  if ((vsp[0] + 0)->_int) {
    vpc[0] += *((int *)vpc[0]);
  } else {
    vpc[0] += 4;
  }
  (vsp[0]) --;
  goto _topOfLoop;

 vcmd_constant_int$result_STA_0$value_LIT_0: /* CIL Label */ ;
  (vpc[0]) ++;
  (vsp[0] + 1)->_int = *((int *)vpc[0]);
  (vsp[0]) ++;
  vpc[0] += 4;
  goto _topOfLoop;

 vcmd_formal$result_STA_0$value_LIT_0: /* CIL Label */ ;
  (vpc[0]) ++;
  switch (*((int *)vpc[0])) {
  case 1:
    (vsp[0] + 1)->_void_star = (void *)(& y);
    break;
  case 0:
    (vsp[0] + 1)->_void_star = (void *)(& x);
    break;
  }
  (vsp[0]) ++;
  vpc[0] += 4;
  goto _topOfLoop;

 vcmd_return_int$expr_STA_0: /* CIL Label */ ;
  (vpc[0]) ++;
  return ((vsp[0] + 0)->_int);
  goto _topOfLoop;

 vcmd_load_int$left_STA_0$result_STA_0: /* CIL Label */ ;
  (vpc[0]) ++;
  (vsp[0] + 0)->_int = *((int *)(vsp[0] + 0)->_void_star);
  goto _topOfLoop;

 vcmd_Lt_int_int2int$left_STA_0$result_STA_0$right_STA_1: /* CIL Label */ ;
  (vpc[0]) ++;
  (vsp[0] + -1)->_int = (vsp[0] + 0)->_int < (vsp[0] + -1)->_int;
  (vsp[0]) --;
  goto _topOfLoop;

 vcmd_label$label_LAB_0: /* CIL Label */ ;
  (vpc[0]) ++;
  goto _topOfLoop;
}


int main(int argc , char **argv )
{
  int x = strtol(argv[1], 0, 0);
  int y = strtol(argv[2], 0, 0);

  return compare(x, y);
}
