#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <libtipu.h>

void
usage(char name[]) {
  printf("IPU on/off control for iMX6\n\n");
  printf("USAGE: %s <on|off>\n", name);
}

int
main(int argc, char *argv[]) {
  int ret;

  if (argc < 2) { usage(argv[0]); return -1; }

  if (!strcmp(argv[1], "on")) {
    ret = ipu_on();

    if (!ret) {
      printf ("LVDS Display Bridge turned on.\n");
    } else {
      printf ("Error code: %d.\n", ret);
    }

    return ret;
  }

  else if (!strcmp(argv[1], "off")) {
    ret = ipu_off();

    if (!ret) {
      printf ("LVDS Display Bridge turned off.\n");
    } else {
      printf ("Error code: %d.\n", ret);
    }

    return ret;
  }

  usage(argv[0]);
  return -1;
}
