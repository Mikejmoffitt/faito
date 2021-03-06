#include "Barge.h"

Barge::Barge(std::string fname)
{
	std::cout << "Installing keyboard... ";
	while (!al_is_keyboard_installed())
	{
		al_install_keyboard(); 
	}
	std::cout << " ok." << std::endl;
	std::cout << "Installing joystick... ";
	al_install_joystick();
	std::cout << " ok." << std::endl;
	joyPad1 = al_get_joystick(0); // We are only concerned with joystick #1
	joyPad2 = al_get_joystick(1); // We are only concerned with joystick #1
	usingJoy1 = false;
	usingJoy2 = false;
	if (joyPad1 != NULL)
	{
		std::cout << "Using joystick 1." << std::endl;
		std::cout << al_get_joystick_name(joyPad1) << std::endl;
		usingJoy1 = true;
	}
	if (joyPad2 != NULL)
	{
		std::cout << "Using joystick 2." << std::endl;
		std::cout << al_get_joystick_name(joyPad2) << std::endl;
		usingJoy2 = true;
	}
	std::cout << "Loading cfg from " << fname << "... ";
	gameCfg = al_load_config_file(fname.c_str());
	if (!gameCfg) // Failed to load from the configuration file
	{
		std::cout << std::endl << "No input config exists. Creating "<< fname << std::endl;
		gameCfg = al_create_config();
		// Tediously build the fresh default configuration for keys...
		setDefaultConfig();
		al_save_config_file(fname.c_str(),gameCfg);
	}
	else
	{
		std::cout << "ok " << std::endl;
	}

	// Copy scancodes and joypad buttons into keys arrays from the config file
	kbKeys1[0] = atoi(al_get_config_value(gameCfg,"keyboard","up1"));
	kbKeys1[1] = atoi(al_get_config_value(gameCfg,"keyboard","down1"));
	kbKeys1[2] = atoi(al_get_config_value(gameCfg,"keyboard","left1"));
	kbKeys1[3] = atoi(al_get_config_value(gameCfg,"keyboard","right1"));
	kbKeys1[4] = atoi(al_get_config_value(gameCfg,"keyboard","a1"));
	kbKeys1[5] = atoi(al_get_config_value(gameCfg,"keyboard","b1"));
	kbKeys1[6] = atoi(al_get_config_value(gameCfg,"keyboard","start1"));
	kbKeys1[7] = atoi(al_get_config_value(gameCfg,"keyboard","sel1"));
	joyKeys1[0] = atoi(al_get_config_value(gameCfg,"joy","a1"));
	joyKeys1[1] = atoi(al_get_config_value(gameCfg,"joy","b1"));
	joyKeys1[2] = atoi(al_get_config_value(gameCfg,"joy","start1"));
	joyKeys1[3] = atoi(al_get_config_value(gameCfg,"joy","sel1"));

	
	kbKeys2[0] = atoi(al_get_config_value(gameCfg,"keyboard","up2"));
	kbKeys2[1] = atoi(al_get_config_value(gameCfg,"keyboard","down2"));
	kbKeys2[2] = atoi(al_get_config_value(gameCfg,"keyboard","left2"));
	kbKeys2[3] = atoi(al_get_config_value(gameCfg,"keyboard","right2"));
	kbKeys2[4] = atoi(al_get_config_value(gameCfg,"keyboard","a2"));
	kbKeys2[5] = atoi(al_get_config_value(gameCfg,"keyboard","b2"));
	kbKeys2[6] = atoi(al_get_config_value(gameCfg,"keyboard","start2"));
	kbKeys2[7] = atoi(al_get_config_value(gameCfg,"keyboard","sel2"));
	joyKeys2[0] = atoi(al_get_config_value(gameCfg,"joy","a2"));
	joyKeys2[1] = atoi(al_get_config_value(gameCfg,"joy","b2"));
	joyKeys2[2] = atoi(al_get_config_value(gameCfg,"joy","start2"));
	joyKeys2[3] = atoi(al_get_config_value(gameCfg,"joy","sel2"));

	std::cout << "Done setting up controller." << std::endl;
}

Barge::~Barge()
{
	if (usingJoy1 || usingJoy2)
	{
		al_uninstall_joystick();
	}
	al_uninstall_keyboard();
	std::cout << "Controller is gone." << std::endl;
}

void Barge::draw(ALLEGRO_BITMAP* chr, int xPos, int yPos, bool flip, bool tint)
{
	if (tint)
	{
		al_draw_tinted_bitmap(chr,al_map_rgb(255,180,255),
			xPos,yPos,flip?ALLEGRO_FLIP_HORIZONTAL:0);
	}
	else
	{
		al_draw_bitmap(chr,xPos,yPos,flip?ALLEGRO_FLIP_HORIZONTAL:0);
	}
}

void Barge::poll()
{
	if (usingJoy1)
	{
		al_get_joystick_state(joyPad1,&joyState); // Poll joystick
	}
	if (al_is_keyboard_installed())
	{
		al_get_keyboard_state(&keyState); // Poll keys
	}
	else
	{
		std::cout << "No keyboard to poll! Reinstalling. " << std::endl;
		al_install_keyboard();
	}

	// Player 1 input processing

	bool joyRight = false;
	bool joyLeft = false;
	bool joyUp = false;
	bool joyDown = false;
	bool joyA = false;
	bool joyB = false;
	bool joyStart = false;
	bool joySel = false;
	
	if (usingJoy1)
	{
		if (joyState.stick[0].axis[0] > 0.5 ||
			joyState.stick[0].axis[2] > 0.5 ||
			joyState.stick[1].axis[0] > 0.5 ||
			joyState.stick[1].axis[2] > 0.5)
		{	
			joyRight = true;
		}
		else
		{
			joyRight = false;
		}
		if (joyState.stick[0].axis[0] < -0.5 ||
			joyState.stick[0].axis[2] < -0.5 ||
			joyState.stick[1].axis[0] < -0.5 ||
			joyState.stick[1].axis[2] < -0.5)
		{
			joyLeft = true;
		}
		else
		{
			joyLeft = false;
		}
		if (joyState.stick[0].axis[1] < -0.5 || 
			joyState.stick[0].axis[3] < -0.5 || 
			joyState.stick[1].axis[1] < -0.5 || 
			joyState.stick[1].axis[3] < -0.5)
		{
			joyUp = true;
		}
		else
		{
			joyUp = false;
		}
		if (joyState.stick[0].axis[1] > 0.5 ||
			joyState.stick[0].axis[3] > 0.5 ||
			joyState.stick[1].axis[1] > 0.5 ||
			joyState.stick[1].axis[3] > 0.5)
		{
			joyDown = true;
		}
		else
		{
			joyDown = false;
		
		}
			joyA = joyState.button[joyKeys1[0]];
			joyB = joyState.button[joyKeys1[1]];
			joyStart = joyState.button[joyKeys1[2]];
			joySel = joyState.button[joyKeys1[3]];
	}
	
	if (al_key_down(&keyState, kbKeys1[0]) || joyUp)
	{
		p1keys[0]++;
	}
	else
	{
		p1keys[0] = 0; // Up
	}
	if (al_key_down(&keyState, kbKeys1[1]) || joyDown)
	{
		p1keys[1]++;
	}
	else
	{
		p1keys[1] = 0; // Down
	}
	if (al_key_down(&keyState, kbKeys1[2]) || joyLeft)
	{
		p1keys[2]++;
	}
	else
	{
		p1keys[2] = 0; // Left
	}
	if (al_key_down(&keyState, kbKeys1[3]) || joyRight)
	{
		p1keys[3]++;
	}
	else
	{
		p1keys[3] = 0; // Right
	}
	if (al_key_down(&keyState, kbKeys1[4]) || joyA)
	{
		p1keys[4]++;
	}
	else
	{
		p1keys[4] = 0; // A
	}
	if (al_key_down(&keyState, kbKeys1[5]) || joyB)
	{
		p1keys[5]++;
	}
	else
	{
		p1keys[5] = 0; // B
	}
	if (al_key_down(&keyState, kbKeys1[6]) || joyStart)
	{
		p1keys[6]++;
	}
	else
	{
		p1keys[6] = 0; // Start
	}
	if (al_key_down(&keyState, kbKeys1[7]) || joySel)
	{
		p1keys[7]++;
	}
	else
	{
		p1keys[7] = 0; // Start
	}

	
	if (usingJoy2)
	{
		al_get_joystick_state(joyPad2,&joyState); // Poll joystick
	}
	// Player 1 input processing

	joyRight = false;
	joyLeft = false;
	joyUp = false;
	joyDown = false;
	joyA = false;
	joyB = false;
	joyStart = false;
	joySel = false;
	
	if (usingJoy2)
	{
		if (joyState.stick[0].axis[0] > 0.5 ||
			joyState.stick[0].axis[2] > 0.5 ||
			joyState.stick[1].axis[0] > 0.5 ||
			joyState.stick[1].axis[2] > 0.5)
		{	
			joyRight = true;
		}
		else
		{
			joyRight = false;
		}
		if (joyState.stick[0].axis[0] < -0.5 ||
			joyState.stick[0].axis[2] < -0.5 ||
			joyState.stick[1].axis[0] < -0.5 ||
			joyState.stick[1].axis[2] < -0.5)
		{
			joyLeft = true;
		}
		else
		{
			joyLeft = false;
		}
		if (joyState.stick[0].axis[1] < -0.5 || 
			joyState.stick[0].axis[3] < -0.5 || 
			joyState.stick[1].axis[1] < -0.5 || 
			joyState.stick[1].axis[3] < -0.5)
		{
			joyUp = true;
		}
		else
		{
			joyUp = false;
		}
		if (joyState.stick[0].axis[1] > 0.5 ||
			joyState.stick[0].axis[3] > 0.5 ||
			joyState.stick[1].axis[1] > 0.5 ||
			joyState.stick[1].axis[3] > 0.5)
		{
			joyDown = true;
		}
		else
		{
			joyDown = false;
		
		}
			joyA = joyState.button[joyKeys1[0]];
			joyB = joyState.button[joyKeys1[1]];
			joyStart = joyState.button[joyKeys1[2]];
			joySel = joyState.button[joyKeys1[3]];
	}
	
	if (al_key_down(&keyState, kbKeys2[0]) || joyUp)
	{
		p2keys[0]++;
	}
	else
	{
		p2keys[0] = 0; // Up
	}
	if (al_key_down(&keyState, kbKeys2[1]) || joyDown)
	{
		p2keys[1]++;
	}
	else
	{
		p2keys[1] = 0; // Down
	}
	if (al_key_down(&keyState, kbKeys2[2]) || joyLeft)
	{
		p2keys[2]++;
	}
	else
	{
		p2keys[2] = 0; // Left
	}
	if (al_key_down(&keyState, kbKeys2[3]) || joyRight)
	{
		p2keys[3]++;
	}
	else
	{
		p2keys[3] = 0; // Right
	}
	if (al_key_down(&keyState, kbKeys2[4]) || joyA)
	{
		p2keys[4]++;
	}
	else
	{
		p2keys[4] = 0; // A
	}
	if (al_key_down(&keyState, kbKeys2[5]) || joyB)
	{
		p2keys[5]++;
	}
	else
	{
		p2keys[5] = 0; // B
	}
	if (al_key_down(&keyState, kbKeys2[6]) || joyStart)
	{
		p2keys[6]++;
	}
	else
	{
		p2keys[6] = 0; // Start
	}
	if (al_key_down(&keyState, kbKeys2[7]) || joySel)
	{
		p2keys[7]++;
	}
	else
	{
		p2keys[7] = 0; // Start
	}
}

#define KB_UP1 ALLEGRO_KEY_UP
#define KB_DOWN1 ALLEGRO_KEY_DOWN
#define KB_LEFT1 ALLEGRO_KEY_LEFT
#define KB_RIGHT1 ALLEGRO_KEY_RIGHT
#define KB_A1 ALLEGRO_KEY_A
#define KB_B1 ALLEGRO_KEY_S
#define KB_START1 ALLEGRO_KEY_1
#define KB_SEL1 ALLEGRO_KEY_5

#define KB_UP2 ALLEGRO_KEY_I
#define KB_DOWN2 ALLEGRO_KEY_K
#define KB_LEFT2 ALLEGRO_KEY_J
#define KB_RIGHT2 ALLEGRO_KEY_L
#define KB_A2 ALLEGRO_KEY_T
#define KB_B2 ALLEGRO_KEY_Y
#define KB_START2 ALLEGRO_KEY_2
#define KB_SEL2 ALLEGRO_KEY_6

#define JOY_A1 0
#define JOY_B1 1
#define JOY_START1 2
#define JOY_SEL1 3

#define JOY_A2 0
#define JOY_B2 1
#define JOY_START2 2
#define JOY_SEL2 3


void Barge::setDefaultConfig()
{
	std::stringstream intStringer;


	intStringer.str("");
	intStringer << KB_UP1;
	al_set_config_value(gameCfg,"keyboard","up1",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_DOWN1;
	al_set_config_value(gameCfg,"keyboard","down1",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_LEFT1;
	al_set_config_value(gameCfg,"keyboard","left1",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_RIGHT1;
	al_set_config_value(gameCfg,"keyboard","right1",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_A1;
	al_set_config_value(gameCfg,"keyboard","a1",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_B1;
	al_set_config_value(gameCfg,"keyboard","b1",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_START1;
	al_set_config_value(gameCfg,"keyboard","start1",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_SEL1;
	al_set_config_value(gameCfg,"keyboard","sel1",intStringer.str().c_str());
	intStringer.str("");
	
	intStringer << JOY_A1;
	al_set_config_value(gameCfg,"joy","a1",intStringer.str().c_str());
	intStringer.str("");
	intStringer << JOY_B1;
	al_set_config_value(gameCfg,"joy","b1",intStringer.str().c_str());
	intStringer.str("");
	intStringer << JOY_START1;
	al_set_config_value(gameCfg,"joy","start1",intStringer.str().c_str());
	intStringer.str("");
	intStringer << JOY_SEL1;
	al_set_config_value(gameCfg,"joy","sel1",intStringer.str().c_str());

	intStringer.str("");
	intStringer << KB_UP2;
	al_set_config_value(gameCfg,"keyboard","up2",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_DOWN2;
	al_set_config_value(gameCfg,"keyboard","down2",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_LEFT2;
	al_set_config_value(gameCfg,"keyboard","left2",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_RIGHT2;
	al_set_config_value(gameCfg,"keyboard","right2",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_A2;
	al_set_config_value(gameCfg,"keyboard","a2",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_B2;
	al_set_config_value(gameCfg,"keyboard","b2",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_START2;
	al_set_config_value(gameCfg,"keyboard","start2",intStringer.str().c_str());
	intStringer.str("");
	intStringer << KB_SEL2;
	al_set_config_value(gameCfg,"keyboard","sel2",intStringer.str().c_str());
	intStringer.str("");

	intStringer << JOY_A2;
	al_set_config_value(gameCfg,"joy","a2",intStringer.str().c_str());
	intStringer.str("");
	intStringer << JOY_B2;
	al_set_config_value(gameCfg,"joy","b2",intStringer.str().c_str());
	intStringer.str("");
	intStringer << JOY_START2;
	al_set_config_value(gameCfg,"joy","start2",intStringer.str().c_str());
	intStringer.str("");
	intStringer << JOY_SEL2;
	al_set_config_value(gameCfg,"joy","sel2",intStringer.str().c_str());
}