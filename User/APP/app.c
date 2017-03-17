#include <includes.h>
//extern void Touch_MainTask(void);
extern void Fatfs_MainTask(void);
void MainAPP(void)
{
	Fatfs_MainTask();
}
