//
// Created by zhc on 2018/5/7
// Created by root on 19-7-3.
//

#ifndef C99_ZHC_H
#define C99_ZHC_H

#include <stdexcept>

#ifndef __cplusplus
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#else

#include "third_party/practice/LinearList.hpp"
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>

using namespace bczhc;
using namespace linearlist;

namespace bczhc {
    using namespace std;
#endif //__cplusplus
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef ARR_len
#define ARR_len(x) sizeof(x) / sizeof(x)[0]
#endif // ARR_len
#ifdef __cplusplus
    extern "C" {
#endif //__cplusplus

    char *ToUpperCase(char *Dest, const char *string);

    void PrintArr(const char arr[], int len);

    int64_t m_pow(int64_t base, int64_t exponent);

    int BinToDec(const char *NumStr);

    void printArr(const char *a, int length);

    char *substring(char *Dest, const char *source, int beginIndex, int endIndex);

    void substr(char **Dest, const char *source, int from, int length);

    char *substr2(char *Dest, const char *source, int from, int length);

    long long getFileSize(FILE *fp);

    int getIntegerLen(int x);

    int getLongLen(long x);

    void Scanf(char **Dest);

    void strcpyAndCat_auto(char **Dest, const char *cpy_s, int cpy_s_length,
                           const char *cat_s, int cat_s_length, bool deleteOld);

    void strcat_auto(char **sourceDest, const char *cat_s);

    void charToCharPtr(char **Dest, char c);
/**
 *
 * @param string s
 * @param s s
 * @return r
 * @example this("123abc123", "23) = 2 this("12342312452312i23ab", "23") = 4
 * usage:
 * int *p = NULL;
 * int t = this(&p, "a1b1c1", "1"); => p[0] = 1, p[1] = 3, p[2] = 5; t = 3;
 */
    uint32_t strInStrCount(int **Dest, const char *string, const char *s);
/**
 * String split
 * @param Dest Dest
 * @param str String
 * @param splitChar as separation
 * @use
 * void ***r = NULL;
 * split(&r, str1, str2);
 * int i = *((int *) (r[0][0]));// element count
 * char **R = ((char **) ((char ***) r)[1]); //char * result
 * for (int j = 0; j < i; ++j) {
 printf("%s\n", R[j]);
 }
 *//*
		  void split(void ****Dest, char *str, const char *splitChar) {
		*Dest = (void ***) malloc((size_t) (sizeof(char **) * 2));
		((*Dest)[0]) = (void **) malloc((size_t) (sizeof(int *) * 1));
		(*Dest)[0][0] = (void *) malloc((size_t) (size_t) (sizeof(int)));
		(*Dest)[1] = (void **) malloc((size_t) (sizeof(void *) * 3));
		char *r = NULL;
		uint32_t sS = strlen(str) + 1, splitChar_s = strlen(splitChar) + 1;
		char str_charArr[sS], splitChar_charArr[splitChar_s];
		for (int j = 0; j < sS - 1; ++j) {
		str_charArr[j] = str[j];
		}
		str_charArr[sS - 1] = 0x0;
		for (int j = 0; j < splitChar_s - 1; ++j) {
		splitChar_charArr[j] = splitChar[j];
		}
		splitChar_charArr[splitChar_s - 1] = 0x0;
		uint32_t eC = strInStrCount(str, splitChar) + 1;
		if (str[0] == splitChar[0]) eC--;
		if (str[sS - 2] == splitChar[splitChar_s - 2]) eC--;
		if (eC != 1) {
		goto n;
		}
		*((int *) ((*Dest)[0][0])) = (int) 0;
		strcpy((*Dest)[1][0], "");
		return;
		n:
		*((int *) ((*Dest)[0][0])) = (int) eC;
		(*Dest)[1] = (void **) malloc((size_t) (sizeof(char *) * eC));
		r = strtok(str_charArr, splitChar_charArr);
		int a_i = 0;
		while (r != NULL) {
		int rS = strlen(r) + 1;
		(*Dest)[1][a_i] = ((char *) malloc((size_t) rS));
		strcpy((*Dest)[1][a_i], r);
		r = strtok(NULL, splitChar_charArr);
		++a_i;
		}
		}*/

    int Str_Cmp_nMatchCase(const char *a, const char *b);

    void m_itoa(char **Dest, int i);

/*void m_lltoa(char **Dest, const int64_t int ll) {

  }*/

    void m_ltoa(char **Dest, long l);

    int split(char ***Dest, const char *SourceString, const char *SplitStr);

    int cmpIntArray(const int *a1, int a1Len, const int *a2, int a2Len);

    int cmpCharArray(const char *a1, int a1Len, const char *a2, int a2Len);

    int charArrToInt(const char *s, size_t size);

    int getBiggerNum(int a, int b);

    int firstIndexOf(const char *s, int s_len, char c);

    char m_itoc(int i);

    int m_ctoi(char c);

    int Split(char ***dst, const char *s, int s_length, const char *separatorString,
              int separatorString_length);
#ifdef __cplusplus
    } // extern C
#endif //__cplusplus

#ifdef __cplusplus

    class PointersSet {
    private:
        SequentialList<void *> list;

    public:
        void put(void *p) { list.insert(p); }

        void freeAll() {
            int length = list.length();
            for (int i = 0; i < length; ++i) {
                free(list.get(i));
            }
        }
    };

#ifdef __cplusplus
} // namespace
#endif // __cplusplus

#endif //__cplusplus

#endif // C99_ZHC_H
