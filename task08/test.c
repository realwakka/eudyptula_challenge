#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void write_foo() {
  int fd;
  char buf[] = "abc";
  fd = open("/sys/kernel/debug/eudyptula/foo", O_RDWR);
  write(fd, buf, 4);
  close(fd);
}

void read_foo() {
  int fd;
  char buf[255];
  fd = open("/sys/kernel/debug/eudyptula/foo", O_RDWR);
  read(fd, buf, 255);
  printf("%s", buf);
  close(fd);
}

int main() {
  //write_foo();
  read_foo();
  

  return 0;
}
