#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define LDB_CTRL    0x020E0008UL

#define IPU_OFF     0x0UL
#define IPU_ON      0x5UL

#define MAP_SIZE    0x1000UL
#define MAP_MASK    (MAP_SIZE - 1)

/**
 * Set the LDB channel state (bits 0-4 in LDB register).
 *
 * Return:
 *   0 : success
 *  -1 : cannot open memory device (/dev/mem)
 *  -2 : cannot map a memory page
 *  -3 : error while unmapping
 */
static int
set_ldb_channel_state(unsigned long state) {
  int fd;
  unsigned long map_base;
  unsigned long ldb_control_register;

  /* open a memory device */
  if (-1 == (fd = open("/dev/mem", O_RDWR | O_SYNC))) { return -1; }

  /* map a memory page */
  if ( -1 == (map_base = (unsigned long)mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, LDB_CTRL & ~MAP_MASK))) { return -2; }

  /**
   * LDB_CTRL = 0x020E0008 
   * MAP_SIZE = 4096 = 0x1000 => MAP_MASK = 0x0FFF
   *
   * LDB_CTRL & ~MAP_MASK = 0x020E0008 & 0xFFFFF000 = 0x020E0000 => base address
   * LDB_CTRL &  MAP_MASK = 0x020E0008 & 0x00000FFF = 0x00000008 => offset address
   */
  #ifdef DEBUG
    printf ("Physical address: 0x%08lx (base: 0x%08lx / offset: 0x%08lx)\n"
      , LDB_CTRL
      , LDB_CTRL & ~MAP_MASK
      , LDB_CTRL & MAP_MASK);
    printf ("Mapped   address: 0x%08lx (base: 0x%08lx / offset: 0x%08lx)\n"
      , map_base + (LDB_CTRL & MAP_MASK)
      , map_base
      , LDB_CTRL & MAP_MASK);
  #endif /* DEBUG */

  /* read the data */
  ldb_control_register = *((unsigned long *)(map_base + (LDB_CTRL & MAP_MASK)));

  #ifdef DEBUG
    printf ("Value of LDB control register: 0x%08lx\n", ldb_control_register);
  #endif /* DEBUG */

  /* mask out channel bits (0-3) */
  ldb_control_register &= 0xFFFFFFF0;

  /* set desired channel state */
  ldb_control_register |= state;

  #ifdef DEBUG
    printf ("Value to be set as LDB control register: 0x%08lx\n", ldb_control_register);
  #endif /* DEBUG */

  /* write the data */
  *((unsigned long *)(map_base + (LDB_CTRL & MAP_MASK))) = ldb_control_register;

  /* unmap the memory */
  if (-1 == munmap((unsigned long *)map_base, MAP_SIZE)) { return -3; }

  /* close the memory device */
  close(fd);

  /* success */
  return 0;
}

int
ipu_on(void) {
  return (set_ldb_channel_state(IPU_ON));
}

int
ipu_off(void) {
  return (set_ldb_channel_state(IPU_OFF));
}
