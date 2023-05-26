void wifi_init();
void wifi_process();

typedef enum _button_interface
{
	NONE,
	B1,
	B2,
}button_interface;

extern uint16_t button_pushed;
