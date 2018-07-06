#include "hardwareinterface.h"
#include <QDebug>

#if defined(Q_OS_LINUX)
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

static bool  s_init_buzzer = 0;

static int _system(const char* cmdstring, char* buf, int len)
{
#if defined(Q_OS_LINUX)
    //qDebug()<<"static int _system(const char* cmdstring, char* buf, int len)";
   int fd[2];
   pid_t pid;
   int n, count;
   memset(buf, 0, len);
   if (pipe(fd) < 0) { return -1; }

   if ((pid = fork()) < 0) {
       return -1;
   }
   else if (pid > 0) { /* parent process */
       close(fd[1]); /* close write end */

       count = 0;
       while ((n = read(fd[0], buf + count, len)) > 0 && count > len) {
           count += n;
       }
       close(fd[0]);

       if (waitpid(pid, NULL, 0) > 0) { return -1; }

   }
   else { /* child process */
       close(fd[0]); /* close read end */
       if (fd[1] != STDOUT_FILENO) {
           if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO) {
               return -1;
           }
           close(fd[1]);
       }
       if (execl("/bin/sh", "sh", "-c", cmdstring, (char*) 0) == -1) {
           return -1;
       }
   }
#endif
   return 0;
}


HardwareInterface::HardwareInterface()
{

}

void HardwareInterface::makeSound()
{
#if defined(Q_OS_LINUX)
    char buf[1];
    if(!s_init_buzzer)
    {
        struct stat st;
        if(0!=stat("sys/class/pwm/pwmchip1/pwm0", &st))
        {
            _system("echo 0 > /sys/class/pwm/pwmchip1/export",buf,1);
        }
        s_init_buzzer = true;
        _system("echo 0 > /sys/class/pwm/pwmchip1/pwm0/duty_cycle",buf,1);
        _system("echo 344826 > /sys/class/pwm/pwmchip1/pwm0/period",buf,1);
        _system("echo 172413 > /sys/class/pwm/pwmchip1/pwm0/duty_cycle",buf,1);
    }
    _system("echo 1 > /sys/class/pwm/pwmchip1/pwm0/enable",buf,1);
    int time = 100000;
    usleep(time);
    _system("echo 0 > /sys/class/pwm/pwmchip1/pwm0/enable",buf,1);
#endif
}

void HardwareInterface::setBacklight(int level)
{
#if defined(Q_OS_LINUX)
    QString cmd = "echo " + QString::number(level) + " > /sys/class/backlight/backlight/brightness";
    char buf[1];
    _system((char*)qPrintable(cmd),buf,1);
#endif
}
