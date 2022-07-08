#ifndef	CPU_H_
#define CPU_H_
/*
 * =====================================================================================
 *
 *       Filename:  cpu.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/05/2018 19:54:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */


#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************
**                           FUNCTION PROTOTYPES
*****************************************************************************/
void CPUSwitchToUserMode(void);
void CPUSwitchToPrivilegedMode(void);

/****************************************************************************/
/*
** Functions used internally
*/
void CPUAbortHandler(void);
void CPUirqd(void);
void CPUirqe(void);
void CPUfiqd(void);
void CPUfiqe(void);
unsigned int CPUIntStatus(void);

#ifdef __cplusplus
}
#endif
#endif /* CPU_H_ */