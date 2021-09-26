#define SWI_STATEFUL_MODEL
#include <6.0g/swi.h>

int main(void) {
    window();
    setVisible(true);
    setSize(640, 480);
    setText("Hello!");
	scibox();
	setSize(640, 480);
	sendmsg(4001, 22, 0);
	sendmsg(2181, 0, (LPARAM)"class A\n    puts \"Hello world\"\nend\n");
	sendmsg(2051, 5, 0xff0000);
	sendmsg(4005, 0, (LPARAM)"__FILE__ and def end in or self unless __LINE__ begin \
defined? ensure module redo super until BEGIN break do false next rescue \
then when END case else for nil retry true while alias class elsif if \
not return undef yield");
	eventloop();
}
