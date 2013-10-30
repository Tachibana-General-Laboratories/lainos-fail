#include <sys/types.h>
#include <sys/stat.h>
#include <user.h>
#include <sys/fcntl.h>

#include <stdlib.h>
#include <string.h>

#define VBE_WIDTH 1024
#define VBE_HEIGHT 768
#define VBE_BYTES 3
#define VBE_SIZE VBE_WIDTH * VBE_HEIGHT * 3

static char *video;
#define PIXEL(x, y) (video + (y * VBE_WIDTH * VBE_BYTES) + (x * VBE_BYTES))

//#define min(a, b) ((a < b)? a: b)
//#define max(a, b) ((a > b)? a: b)

void swap(int *x, int *y) {
	if (x != y) {
		*x ^= *y;
		*y ^= *x;
		*x ^= *y;
	}
}

#define setPixel(x, y, color) \
	if(x>0 && y>0 && x<VBE_WIDTH && y<VBE_HEIGHT)\
		*(int *)PIXEL(x, y) = color & 0xFFFFFF;

#define abs(a) ((a > 0)? a: -a)
void drawLine(int x1, int y1, int x2, int y2) {
	if(x1>x2) swap(&x1, &x2);
	if(y1>y2) swap(&y1, &y2);

    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    //
    int error = deltaX - deltaY;
    //
    setPixel(x2, y2, 0x00CCCC);

    while(x1 != x2 || y1 != y2) {
        setPixel(x1, y1, 0x00CCCC);
        const int error2 = error * 2;
        //
        if(error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
}

void drawRect(int x1, int y1, int x2, int y2) {
	if(x1>x2) swap(&x1, &x2);
	if(y1>y2) swap(&y1, &y2);

	int x, y;
	for(int y = y1; y<y2; y++) {
		for(int x = x1; x<x2; x++) {
			setPixel(x, y, 0x0000CC);
		}
	}
}


#define memswap(fd) \
	if(lseek(fd, 0, SEEK_SET) != -1) \
		write(fd, video, VBE_SIZE); \
	else printf(2, "fail lseek\n");

#undef memswap
#define memswap(fd) \
	lseek(fd, 0, SEEK_SET); \
	write(fd, video, VBE_SIZE);


int main() {
	int i;
	video = malloc(VBE_SIZE);
	memset(video, 0, VBE_SIZE);

	int vbe_fd;
	if ((vbe_fd = open("/dev/vbe", O_RDWR)) < 0) {
		printf(2, "fail\n");
		return 1;
	}
/*
	lseek(vbe_fd, 0, SEEK_END);
	read(vbe_fd, video, VBE_SIZE);
	*/

	for(i=0; i<70; i++) {
		drawLine(30+i*10, 30+i*5, 500-i*5, 600-i*10);
		memswap(vbe_fd);
	}
	i=33;
	printf(1, "fail %d %d %d %d\n", 30+i*10, 30+i*5, 500-i*5, 600-i*10);

	if(0) {
		int x1=360; int y1=195; int x2=335; int y2=270;
		const int deltaX = abs(x2 - x1);
		const int deltaY = abs(y2 - y1);
		const int signX = x1 < x2 ? 1 : -1;
		const int signY = y1 < y2 ? 1 : -1;
		//
		int error = deltaX - deltaY;
		//
		printf(1, "se %d %d\n", x2,y2);
		setPixel(x2, y2, 0x00CCCC);

		while(x1 != x2 || y1 != y2) {
			printf(1, "se %d^%d %d^%d\n", x1,x2, y1, y2);
			setPixel(x1, y1, 0x00CCCC);
			const int error2 = error * 2;
			//
			if(error2 > -deltaY) {
				error -= deltaY;
				x1 += signX;
			}
			if(error2 < deltaX) {
				error += deltaX;
				y1 += signY;
			}
		}
	}

	drawRect(500, 300, 750, 600);
	memswap(vbe_fd);
/*
	lseek(vbe_fd, 0, SEEK_END);
	write(vbe_fd, video, VBE_SIZE);
*/

	/*for(i = 0; i < 100; i++) {
		memswap(vbe_fd);
	}*/



	return 0;
}
