/*
 * @Descripttion: 
 * @Version: 
 * @Author: HeyRoo
 * @Date: 2021-06-29 18:19:13
 * @LastEditors: HeyRoo
 * @LastEditTime: 2021-06-29 18:26:13
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main()
{
   char *str;
 
   /* 最初的内存分配 */
   str = (char *) malloc(15);
   strcpy(str, "runoob");
   printf("String = %s,  Address = %p\n", str, str);
   printf("%c\n", str[14]);
   /* 重新分配内存 */
   str = (char *) realloc(str, 3);
   strcat(str, ".com");
   printf("String = %s,  Address = %p\n", str, str);
   printf("%c\n", str[14]);
   // printf("%ld\n", (sizeof(str) / sizeof(char)));
   free(str);
   
   return(0);
}