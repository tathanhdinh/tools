/* Generated by CIL v. 1.7.0 */
/* print_CIL_Input is false */
#include <stdlib.h>

int compare(int x , int y ) ;

enum vcmd$op {
  vcmd_formal$result_STA_0$value_LIT_0                    = 192,
  vcmd_Gt_int_int2int$right_STA_0$result_STA_0$left_STA_1 = 2,
  vcmd_constant_int$result_STA_0$value_LIT_0              = 253,
  vcmd_goto$label_LAB_0                                   = 149,
  vcmd_Lt_int_int2int$left_STA_0$result_STA_0$right_STA_1 = 165,
  vcmd_label$label_LAB_0                                  = 114,
  vcmd_return_int$expr_STA_0                              = 127,
  vcmd_load_int$left_STA_0$result_STA_0                   = 82,
  vcmd_branchIfTrue$expr_STA_0$label_LAB_0                = 184
} ;

unsigned char vcodes[1][70]  =
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


union vcmd$node
{
  void *_void_star;
  int _int;
};


int main(int argc , char *argv[])
{
  int x = strtol(argv[1], 0, 0);
  int y = strtol(argv[2], 0, 0);

  return compare(x, y);
}


int compare(int x , int y )
{
  union vcmd$node vstack[1][32];
  union vcmd$node *vsp[1];
  unsigned char *vpc[1];

  vsp[0] = vstack[0];
  vpc[0] = vcodes[0];

  while (1) {
    switch (*(vpc[0])) {
    case vcmd_formal$result_STA_0$value_LIT_0:
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
      break;

    case vcmd_load_int$left_STA_0$result_STA_0:
      (vpc[0]) ++;
      (vsp[0] + 0)->_int = *((int *)(vsp[0] + 0)->_void_star);
      break;

    case vcmd_constant_int$result_STA_0$value_LIT_0:
      (vpc[0]) ++;
      (vsp[0] + 1)->_int = *((int *)vpc[0]);
      (vsp[0]) ++;
      vpc[0] += 4;
      break;

    case vcmd_Gt_int_int2int$right_STA_0$result_STA_0$left_STA_1:
      (vpc[0]) ++;
      (vsp[0] + -1)->_int = (vsp[0] + -1)->_int > (vsp[0] + 0)->_int;
      (vsp[0]) --;
      break;

    case vcmd_Lt_int_int2int$left_STA_0$result_STA_0$right_STA_1:
      (vpc[0]) ++;
      (vsp[0] + -1)->_int = (vsp[0] + 0)->_int < (vsp[0] + -1)->_int;
      (vsp[0]) --;
      break;

    case vcmd_return_int$expr_STA_0:
      (vpc[0]) ++;
      return ((vsp[0] + 0)->_int);
      break;

    case vcmd_goto$label_LAB_0:
      (vpc[0]) ++;
      vpc[0] += *((int *)vpc[0]);
      break;

    case vcmd_label$label_LAB_0:
      (vpc[0]) ++;
      break;

    case vcmd_branchIfTrue$expr_STA_0$label_LAB_0:
      (vpc[0]) ++;
      if ((vsp[0] + 0)->_int) {
        vpc[0] += *((int *)vpc[0]);
      } else {
        vpc[0] += 4;
      }
      (vsp[0]) --;
      break;
    }
  }
}

