#include QMK_KEYBOARD_H



//Define custom keycodes
enum custom_keycodes {		//_tap_action_ | _hold_action_
	APPBACKX = SAFE_RANGE,	//app back | close app
	APPFWRDN,		//app forward | new app
	BACKBACK,		//back | back
	BKSPWORD,		//backspace | backspace word
	CADQBOOT,		//control/alt/delete | quantum keyboard boot
	CLIKLOCK,		//click lock
	COPY_CUT,		//copy | cut
	CPI_DOWN,		//CPI down
	CPI_UPUP,		//CPI up
	CPIRESET,		//CPI set/get | CPI display
	DEL_WORD,		//delete | delete word
	DOWNPGDN,		//down | page down
	ENTERSFT,		//enter | shift
	EXITLYR2,		//exit layer 2
	EXL5MOL6,		//exit layer 5 | momentary layer 6
	FILESALL,		//file | select all
	FWRDFWRD,		//forward | forward
	INFOOPEN,		//info | open
	LEFTHOME,		//left | home
	LYR3SHFT,		//to layer 3 | shift
	LYR5CTRL,		//to layer 5 | control
	MEDIATGL,		//volume toggle
	MIDLHOLD,		//Middle click | hold click
	OSTOGGLE,		//OS toggle | OS display
	OVERVIEW,		//overview
	PRNTSCRN,		//print screen | print
	PSTPLAIN,		//paste | paste plain text
	REDONEXT,		//redo | next
	RENAMEFL,		//rename file
	RFRSRFRS,		//refresh | refresh
	RIGHTEND,		//right | end
	SAVEDNLD,		//save | download
	SCRLAXIS,		//scroll axis
	SPC_NBSP,		//space | non-breaking space
	SRCHSRCH,		//search | search
	TABBACKX,		//tab back | close tab
	TABFWRDN,		//tab forward | new tab
	TABSHIFT,		//tab | shift+tab
	UNDOPREV,		//undo | previous
	UPPAGEUP,		//up | page up
	VDESKBKX,		//virtual desktop back | close virtual desktop
	VDESKFWN,		//virtual desktop forward | new virtual desktop
	ZOOMTGGL		//zoom toggle
};



//Define combos
const uint16_t PROGMEM combo_clip[] = {COPY_CUT, PSTPLAIN, COMBO_END};
const uint16_t PROGMEM combo_cpir[] = {CPI_DOWN, CPI_UPUP, COMBO_END};
const uint16_t PROGMEM combo_incg[] = {TABFWRDN, APPFWRDN, COMBO_END};
const uint16_t PROGMEM combo_lyr8[] = {LYR3SHFT, LYR5CTRL, COMBO_END};
const uint16_t PROGMEM combo_rlct[] = {TABBACKX, TABFWRDN, COMBO_END};
combo_t key_combos[] = {
    COMBO(combo_clip, G(KC_V)),		//Open clipboard history
    COMBO(combo_cpir, CPIRESET),	//Reset CPI
    COMBO(combo_incg, C(S(KC_N))),	//Open a new incognito window
    COMBO(combo_lyr8, OSL(8)),		//One-shot layer 8
    COMBO(combo_rlct, C(S(KC_T))),	//Reopen last closed tab
};



//Define variables used in custom keycode and custom scroll mode handling
static bool alt_held = false;		//alt held tracker
static bool d_scroll = false;		//dual/diagonal axis scroll lock
static bool h_scroll = false;		//horizontal scroll lock
static bool v_scroll = false;		//vertical scroll lock
static bool mediatgl = false;		//media control lock
static bool noscroll = false;		//no scroll (cursor movement) lock
static bool zoomtggl = false;		//zoom lock
static uint8_t os = 0;			//operating system tracker
static uint16_t cpi = 800;		//trackball sensor count per inch setting tracker
static uint16_t mediatgl_timer = 0;	//timer used to regulate mouse movement for media control
static uint16_t tap_hold_timer = 0;	//timer used to distinguish taps from holds



//Handle OS detection
bool process_detected_host_os_user(os_variant_t detected_os) {
	switch (detected_os) {
		case OS_UNSURE:
			os = 0;		//OS: unknown
			return false;
		case OS_WINDOWS:
			os = 1;		//OS: Windows
			return false;
		case OS_MACOS:
			os = 2;		//OS: macOS
			return false;
		case OS_IOS:
			os = 3;		//OS:iOS
			return false;
		case OS_LINUX:
			//os = 4;	//OS: Android
			os = 5;		//OS: chromeOS
			//os = 6;	//OS: Linux with GNOME
			return false;	}
	return true;
}



////////////////////////////////
/////Handle custom keycodes/////
////////////////////////////////
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
/*
//TEMPLATE: TAP-HOLD CONFIGS
		case TEMPLATE:
			if (record->event.pressed) {
				//THIS CODE WILL EXECUTE AT THE BEGINNING OF EVERY TAP OR HOLD
				tap_hold_timer = timer_read();
				//MORE THAN JUST THE TIMER READ CAN HAPPEN HERE
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					//THIS CODE WILL EXECUTE ONCE FOR EVERY TAP
				} else {
					//THIS CODE WILL EXECUTE AT THE END OF EVERY HOLD
				}
			}
			return false; //'return false;' IS EFFECTIVELY ALMOST THE SAME AS 'break:' BUT 'break;' TURNED OUT TO CAUSE DOUBLE EVENTS FOR ME IN SOME CASES
*/
//APPBACKX tap: Cycle right through open apps [alt + shift + tab]
//APPBACKX hold (Windows w/ app switcher open): Close the currently selected app [del]
//APPBACKX hold (chromeOS w/ app switcher open): Close currently selected app [unregister alt | ctrl + shift + W | alt + tab]
//APPBACKX hold (GNOME w/ app switcher open): Close currently selected app [unregisted alt | alt + F4 | tab]
//APPBACKX hold (Windows & GNOME w/o app switcher open): Close current application [alt + F4]
//APPBACKX hold (chromeOS w/o app switcher open): Close current application [ctrl + shift + W]
		case APPBACKX:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					if (alt_held) {
						register_code(KC_LSFT);
						wait_ms(10);
						tap_code(KC_TAB);
						unregister_code(KC_LSFT);
					} else {
						alt_held = true;
						register_code(KC_LALT);
						wait_ms(10);
						register_code(KC_LSFT);
						wait_ms(10);
						tap_code(KC_TAB);
						unregister_code(KC_LSFT);
					}
				} else {
					if (alt_held) {
						if (os == 1) {
							tap_code(KC_DEL);
						} else if (os == 5) {
							unregister_code(KC_LALT);
							wait_ms(50);
							register_code(KC_LCTL);
							wait_ms(10);
							register_code(KC_LSFT);
							wait_ms(10);
							tap_code(KC_W);
							unregister_code(KC_LSFT);
							unregister_code(KC_LCTL);
							wait_ms(50);
							register_code(KC_LALT);
							wait_ms(10);
							tap_code(KC_TAB);
						} else if (os == 6) {
							unregister_code(KC_LALT);
							wait_ms(50);
							register_code(KC_LALT);
							wait_ms(10);
							tap_code(KC_F4);
							wait_ms(10);
							tap_code(KC_TAB);
						}
					} else if (os == 1 || os == 6) {
						register_code(KC_LALT);
						wait_ms(10);
						tap_code(KC_F4);
						unregister_code(KC_LALT);
					} else if (os == 5) {
						register_code(KC_LCTL);
						wait_ms(10);
						register_code(KC_LSFT);
						wait_ms(10);
						tap_code(KC_W);
						unregister_code(KC_LSFT);
						unregister_code(KC_LCTL);
					}
				}
			}
			return false;
//APPFWRDN tap: Cycle left through open apps [alt + tab]
//APPFWRDN hold: Open new application [ctrl + N], then go to base layer
		case APPFWRDN:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					if (alt_held) {
						tap_code(KC_TAB);
					} else {
						alt_held = true;
						register_code(KC_LALT);
						wait_ms(10);
						tap_code(KC_TAB);
					}
				} else {
					if (alt_held) {
						alt_held = false;
						unregister_code(KC_LALT);
					}
					wait_ms(50);
					tap_code16(C(KC_N));
					layer_move(0);
				}
			}
			return false;
//BACKBACK tap: Back [mouse button 4]
//BACKBACK hold: Back [WWW back keycode]
		case BACKBACK:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_MS_BTN4);
				} else {
					tap_code(KC_WBAK);
				}
			}
			return false;
//BKSPWORD tap: Backspace
//BKSPWORD hold: Backspace one word at a time [ctrl + backspace]
		case BKSPWORD:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_BSPC);
				} else {
					tap_code16(C(KC_BSPC));
				}
			}
			return false;
//CADQBOOT tap: [ctrl + alt + del]
//CADQBOOT hold: Put the mouse into bootloader mode
		case CADQBOOT:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code16(C(A(KC_DEL)));
				} else {
					reset_keyboard();
				}
			}
			return false;
//COPY_CUT tap: Copy [ctrl + ins]
//COPY_cut hold: Cut [ctrl + X]
		case COPY_CUT:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code16(C(KC_INS));
				} else {
					tap_code16(C(KC_X));
				}
			}
			return false;
//————— PMW-3360 sensor: The CPI range is 100-12000, in increments of 100 —————
//CPI_DOWN tap: Divide the current mouse CPI in half, down to 100
//CPI_DOWN hold: Set the CPI to 100
		case CPI_DOWN:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					cpi = pointing_device_get_cpi();
					if (cpi >= 200) {
						cpi = cpi / 2;
					}
					pointing_device_set_cpi(cpi);
				} else {
					cpi = 100;
					pointing_device_set_cpi(cpi);
				}
				
			}
			return false;
//CPI_UPUP tap: Double current mouse CPI, up to 3200
//CPI_UPUP hold: Set the CPI to 3200
		case CPI_UPUP:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					cpi = pointing_device_get_cpi();
					if (cpi <= 1600) {
						cpi = cpi * 2;
					}
					pointing_device_set_cpi(cpi);
				} else {
					cpi = 3200;
					pointing_device_set_cpi(cpi);
				}
			}
			return false;
//CPIRESET tap: Reset mouse CPI to 800
		case CPIRESET:
			if (record->event.pressed) {
				cpi = 800;
				pointing_device_set_cpi(cpi);
			}
			return false;
//DEL_WORD tap: Forward delete [del]
//DEL_WORD hold: Forward delete one word at a time [ctrl + del]
		case DEL_WORD:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_DEL);
				} else {
					tap_code16(C(KC_DEL));
				}
			}
			return false;
//DOWNPGDN tap: [down arrow]
//DOWNPGDN hold: [page down]
		case DOWNPGDN:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_DOWN);
				} else {
					tap_code(KC_PGDN);
				}
			}
			return false;
//ENTERSFT tap: [enter]
//ENTERSFT hold: soft line break [shift + enter]
		case ENTERSFT:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_ENT);
				} else {
					tap_code16(S(KC_ENT));
				}
			}
			return false;
//EXITLYR2 tap: Reset noscroll, mediatgl, & zoomtggl, then go to base layer
//EXITLYR2 hold: At the beginning of the hold, enable mouse X & Y movement rather than scrolling; return to scrolling upon release
		case EXITLYR2:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
				noscroll = true;
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					noscroll = false;
					mediatgl = false;
					zoomtggl = false;
					unregister_mods(MOD_BIT(KC_LCTL));
					layer_move(0);
				} else {
					noscroll = false;
				}
			}
			return false;
//EXL5MOL6 tap: Go to base layer
//EXL5MOL6 hold: Go to virtual desktop layer while held; go to base layer when released
		case EXL5MOL6:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
				if (alt_held) {
					alt_held = false;
					unregister_code(KC_LALT);
				}
				layer_move(6);
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					layer_move(0);
				} else {
					layer_move(0);
				}
			}
			return false;
//FILESALL tap (Windows & GNOME): Open file browser [gui + E]
//FILESALL tap (chromeOS): Open file browser [alt + shift + M]
//FILESALL hold: Select all [ctrl + A]
		case FILESALL:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					if (os == 1 || os == 6) {
						tap_code16(G(KC_E));
					} else if (os == 5) {
						tap_code16(A(S(KC_M)));
					}
				} else {
					tap_code16(C(KC_A));
				}
			}
			return false;
//FWRDFWRD tap: Forward [mouse button 5]
//FWRDFWRD hold: Forward [WWW forward keycode]
		case FWRDFWRD:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_MS_BTN5);
				} else {
					tap_code(KC_WFWD);
				}
			}
			return false;
//INFOOPEN tap (Windows & GNOME): File properties [alt + ent]
//Note: [alt + ent] also creates a line break within a text field in some applications
//INFOOPEN tap (chromeOS): File info [space]
//INFOOPEN hold: Open [ctrl + O]
		case INFOOPEN:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					if (os == 1 || os == 6) {
						tap_code16(A(KC_ENT));
					} else if (os == 5) {
						tap_code(KC_SPC);
					}
				} else {
					tap_code16(C(KC_O));
				}
			}
			return false;
//LEFTHOME tap: [left arrow]
//LEFTHOME hold: [home]
		case LEFTHOME:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_LEFT);
				} else {
					tap_code(KC_HOME);
				}
			}
			return false;
//LYR3SHFT tap: Go to edit layer
//LYR3SHFT hold: [shift]
		case LYR3SHFT:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
				register_mods(MOD_BIT(KC_LSFT));
			} else {
				unregister_mods(MOD_BIT(KC_LSFT));
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					layer_move(3);
				}
			}
			return false;
//LYR5CTRL tap: Go to apps/tabs layer
//LYR5CTRL hold: [ctrl]
		case LYR5CTRL:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
				register_mods(MOD_BIT(KC_LCTL));
			} else {
				unregister_mods(MOD_BIT(KC_LCTL));
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					layer_move(5);
				}
			}
			return false;
//MEDIATGL tap: Reset zoom lock, then toggle media control lock for trackball movement
		case MEDIATGL:
			if (record->event.pressed) {
				zoomtggl = false;
				unregister_mods(MOD_BIT(KC_LCTL));
				mediatgl = !mediatgl;
			}
			return false;
//MIDLHOLD click: Middle click
//MIDLHOLD hold: Register a click and hold of mouse button 1, useful for multi-select or for drag-and-drop 
		case MIDLHOLD:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_BTN3);
				} else {
					register_code(KC_BTN1);
				}
			}
			return false;
//OSTOGGLE tap: Update OS variable
//OSTOGGLE hold: Display currently configured OS
		case OSTOGGLE:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					os = (os + 1) % 7;
				} else {
					switch (os) {
						case 1:
							SEND_STRING("Host OS is currently set to 'Windows'\n");
							return false;
						case 2:
							SEND_STRING("Host OS is currently set to 'macOS'\n");
							return false;
						case 3:
							SEND_STRING("Host OS is currently set to 'iOS'\n");
							return false;
						case 4:
							SEND_STRING("Host OS is currently set to 'Android'\n");
							return false;
						case 5:
							SEND_STRING("Host OS is currently set to 'chromeOS'\n");
							return false;
						case 6:
							SEND_STRING("Host OS is currently set to 'Linux'\n");
							return false;
						default:
							SEND_STRING("Host OS is currently set to 'Unknown'\n");
							return false;
					}
				}
			}
			return false;
//OVERVIEW tap (Windows): Open virtual desktops overview [gui + tab], then go to base layer
//OVERVIEW tap (chromeOS): Nothing — [alt held + tab] (same as cycling through tabs) is the only shortcut found, but unregistering alt closes it
//OVERVIEW tap (GNOME): Nothing — no good keyboard shortcut found
		case OVERVIEW:
			if (os == 1) {
				if (alt_held) {
					alt_held = false;
					unregister_code(KC_LALT);
					wait_ms(10);
				}
				register_code(KC_LGUI);
				wait_ms(10);
				tap_code(KC_TAB);
				unregister_code(KC_LGUI);
				layer_move(0);
			}
			break;
//PRNTSCRN tap: Print screen [pscr]
//PRNTSCRN hold: Print [ctrl + P]
		case PRNTSCRN:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_PSCR);
				} else {
					tap_code16(C(KC_P));
				}
			}
			return false;
//PSTPLAIN tap: Paste [shift + ins]
//PSTPLAIN hold: Paste without formatting/paste plain text [ctrl + shift + V]
		case PSTPLAIN:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code16(S(KC_INS));
				} else {
					tap_code16(C(S(KC_V)));
				}
			}
			return false;
//REDONEXT tap: Redo [ctrl + Y]
//REDONEXT hold: Media next track
		case REDONEXT:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code16(C(KC_Y));
				} else {
					tap_code(KC_MNXT);
				}
			}
			return false;
//RENAMEFL tap (Windows & GNOME): Rename selected file [F2]
//RENAMEFL tap (chromeOS): Rename selected file [ctrl + enter]
		case RENAMEFL:
			if (os == 1 || os == 6) {
				tap_code(KC_F2);
			} else if (os == 5) {
				tap_code16(C(KC_ENT));
			}
			break;
//RFRSRFRS tap: Refresh [ctrl + R]
//RFRSRFRS hold: Refresh [WWW refresh keycode]
		case RFRSRFRS:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code16(C(KC_R));
				} else {
					tap_code(KC_WREF);
				}
			}
			return false;
//RIGHTEND tap: [right arrow]
//RIGHTEND hold: end]
		case RIGHTEND:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_RGHT);
				} else {
					tap_code(KC_END);
				}
			}
			return false;
//SAVEDNLD tap: Save [ctrl + S]
//SAVEDNLD hold: Download [alt + left click]
		case SAVEDNLD:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code16(C(KC_S));
				} else {
					register_code(KC_LALT);
					wait_ms(10);
					register_code(KC_BTN1);
					wait_ms(10);
					unregister_code(KC_BTN1);
					wait_ms(10);
					unregister_code(KC_LALT);

				}
			}
			return false;
//SCRLAXIS: First, reset media control lock and zoom lock
//SCRLAXIS tap: Enable default of snap scrolling mode by resetting vertical, horizontal, and dual/diagonal axis scroll locks
//SCRLAXIS hold: Cycle through vertical scroll lock, horizontal scroll lock, and dual/diagonal scroll lock
		case SCRLAXIS:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
				mediatgl = false;
				zoomtggl = false;
				unregister_mods(MOD_BIT(KC_LCTL));
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					v_scroll = false;
					h_scroll = false;
					d_scroll = false;
				} else {
					if (!v_scroll && !h_scroll && !d_scroll) {
						v_scroll = true;
					} else if (v_scroll) {
						v_scroll = false;
						h_scroll = true;
					} else if (h_scroll) {
						h_scroll = false;
						d_scroll = true;
					} else if (d_scroll) {
						d_scroll = false;
						v_scroll = true;
					}
				}
			}
			return false;
//SPC_NBSP tap: [space]
//SPC_NBSP hold (Windows): Non-breaking space [alt + 0160]
//SPC_NBSP hold (chromeOS & Linux): Non-breaking space [U + 00a0]
		case SPC_NBSP:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_SPC);
				} else {
					if (os == 1) {
						SEND_STRING(SS_LALT(SS_TAP(X_KP_0) SS_TAP(X_KP_1) SS_TAP(X_KP_6) SS_TAP(X_KP_0) ));
					} else if (os == 5 || os == 6) {
						SEND_STRING(SS_LCTL(SS_LSFT(SS_TAP(X_U))) SS_DELAY(100) SS_TAP(X_0) SS_DELAY(100) SS_TAP(X_0) SS_DELAY(100) SS_TAP(X_A) SS_DELAY(100) SS_TAP(X_0)  SS_DELAY(100) SS_TAP(X_ENTER));
					}
				}
			}
			return false;
//SRCHSRCH tap: Search [ctrl + L]
//SRCHSRCH hold: Search [WWW search keycode]
		case SRCHSRCH:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code16(C(KC_L));
				} else {
					tap_code(KC_WSCH);
				}
			}
			return false;
//TABBACKX tap: Cycle left through tabs in current app [ctrl + shift + tab]
//TABBACKX hold: Close the current tab [ctrl + W]
		case TABBACKX:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
				if (alt_held) {
					alt_held = false;
					unregister_code(KC_LALT);
				}
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					register_code(KC_LCTL);
					wait_ms(10);
					register_code(KC_LSFT);
					wait_ms(10);
					tap_code(KC_TAB);
					unregister_code(KC_LSFT);
					unregister_code(KC_LCTL);
				} else {
					register_code(KC_LCTL);
					wait_ms(10);
					tap_code(KC_W);
					unregister_code(KC_LCTL);
				}
			}
			return false;
//TABFWRDN tap: Cycle right through tabs in current app [ctrl + tab]
//TABFWRDN hold: Open new tab [ctrl + T], then go to base layer
		case TABFWRDN:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
				if (alt_held) {
					alt_held = false;
					unregister_code(KC_LALT);
				}
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					register_code(KC_LCTL);
					wait_ms(10);
					tap_code(KC_TAB);
					unregister_code(KC_LCTL);
				} else {
					register_code(KC_LCTL);
					wait_ms(10);
					tap_code(KC_T);
					unregister_code(KC_LCTL);
					layer_move(0);
				}
			}
			return false;
//TABSHIFT tap: [tab]
//TABSHIFT hold: [shift + tab]
		case TABSHIFT:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_TAB);
				} else {
					tap_code16(S(KC_TAB));
				}
			}
			return false;
//UNDOPREV tap: Undo [ctrl + Z]
//UNDOPREV hold: Media previous track
		case UNDOPREV:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code16(C(KC_Z));
				} else {
					tap_code(KC_MPRV);
				}
			}
			return false;
//UPPAGEUP tap: [arrow up]
//UPPAGEUP hold: [page up]
		case UPPAGEUP:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					tap_code(KC_UP);
				} else {
					tap_code(KC_PGUP);
				}
			}
			return false;
//VDESKBKX tap (Windows): Cycle right through virtual desktops [gui + ctrl + left arrow]
//VDESKBKX tap (chromeOS): Cycle right through virtual desktops [ gui + '[' ]
//VDESKBKX tap (GNOME): Cycle right through virtual desktops [gui + pgup]
//VDESKBKX hold (Windows): Close current virtual desktop [gui + ctrl + F4]
//VDESKBKX hold (chromeOS): Close current virtual desktop [gui + shift + '-']
//VDESKBKX hold (GNOME): Nothing; no good keyboard shortcut found
		case VDESKBKX:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					if (os == 1) {
						register_code(KC_LGUI);
						wait_ms(10);
						register_code(KC_LCTL);
						wait_ms(10);
						tap_code(KC_LEFT);
						unregister_code(KC_LCTL);
						unregister_code(KC_LGUI);
					} else if (os == 5) {
						register_code(KC_LGUI);
						wait_ms(10);
						tap_code(KC_LBRC);
						unregister_code(KC_LGUI);
					} else if (os == 6) {
						register_code(KC_LGUI);
						wait_ms(10);
						tap_code(KC_PGUP);
						unregister_code(KC_LGUI);
					}
				} else {
					if (os == 1) {
						register_code(KC_LGUI);
						wait_ms(10);
						register_code(KC_LCTL);
						wait_ms(10);
						tap_code(KC_F4);
						unregister_code(KC_LCTL);
						unregister_code(KC_LGUI);
					} else if (os == 5) {
						register_code(KC_LGUI);
						wait_ms(10);
						register_code(KC_LSFT);
						wait_ms(10);
						tap_code(KC_MINS);
						unregister_code(KC_LSFT);
						unregister_code(KC_LGUI);
					}
				}
			}
			return false;
//VDESKFWN tap (Windows): Cycle left through virtual desktops [gui + control + right]
//VDESKFWN tap (chromeOS): Cycle left through virtual desktops [ gui + ']' ]
//VDESKFWN tap (GNOME): Cycle left through virtual desktops [gui + pgdn]
//VDESKFWN hold (Windows): New virtual desktop [gui + ctrl + D]
//VDESKFWN hold (chromeOS): New virtual desktop [gui + shift + '=']
//VDESKFWN hold (GNOME): Nothing; no good shortcut found
		case VDESKFWN:
			if (record->event.pressed) {
				tap_hold_timer = timer_read();
			} else {
				if (timer_elapsed(tap_hold_timer) < TAPPING_TERM) {
					if (os == 1) {
						register_code(KC_LGUI);
						wait_ms(10);
						register_code(KC_LCTL);
						wait_ms(10);
						tap_code(KC_RGHT);
						unregister_code(KC_LCTL);
						unregister_code(KC_LGUI);
					} else if (os == 5) {
						register_code(KC_LGUI);
						wait_ms(10);
						tap_code(KC_RBRC);
						unregister_code(KC_LGUI);
					} else if (os == 6) {
						register_code(KC_LGUI);
						wait_ms(10);
						tap_code(KC_PGDN);
						unregister_code(KC_LGUI);
					}
				} else {
					if (os == 1) {
						register_code(KC_LGUI);
						wait_ms(10);
						register_code(KC_LCTL);
						wait_ms(10);
						tap_code(KC_D);
						unregister_code(KC_LCTL);
						unregister_code(KC_LGUI);
					} else if (os == 5) {
						register_code(KC_LGUI);
						wait_ms(10);
						register_code(KC_LSFT);
						wait_ms(10);
						tap_code(KC_EQL);
						unregister_code(KC_LSFT);
						unregister_code(KC_LGUI);
					}
				}
			}
			return false;
//ZOOMTGGL tap: Toggle zoom lock on [register ctrl] (after resetting horizontal scroll lock and media control lock) or off [unregister ctrl]
//Note: [ctrl] registered will translate scrolling up and down to zooming in and out in most applications
		case ZOOMTGGL:
			if (record->event.pressed) {
				if (zoomtggl) {
					zoomtggl = false;
					unregister_mods(MOD_BIT(KC_LCTL));
				} else {
					mediatgl = false;
					h_scroll = false;
					zoomtggl = true;
					register_mods(MOD_BIT(KC_LCTL));
				}
			}
			return false;
//
	}
	return true;
}
////////////////////////////////////////
/////End of custom keycode handling/////
////////////////////////////////////////



//Handle layer state
layer_state_t layer_state_set_user(layer_state_t state) {
//Reset ctrl, zoomtggl, and mediatgl when leaving scroll layer
//Note: This is necessary in case zoomtggl or mediatgl is set while accessing the scroll layer by holding the tap-toggle key on the base layer
	if (!layer_state_cmp(state, 2)) {
		mediatgl = false;
		zoomtggl = false;
		unregister_mods(MOD_BIT(KC_LCTL));
	}
	return state;
}



//Handle mouse movement
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
uint8_t current_layer = get_highest_layer(layer_state);
	//Trackball movement from alt ball layer
	if (current_layer == 2) {
		//If no scroll lock "noscroll" is on, treat trackball movement as normal cursor movement
		//Note: This allows for quick movements of the cursor while locked in the scroll layer
		if (noscroll) {
			mouse_report.h = 0;
			mouse_report.v = 0;
		//If media control lock "mediatgl" is on, trackball movement will control media
		} else if (mediatgl) {
			uint16_t now = timer_read();
			if (timer_elapsed(mediatgl_timer) > 150) {
				//Vertical movement controls volume
				if (mouse_report.y <= -2) {
					tap_code(KC_VOLU);
					mediatgl_timer = now;
				} else if (mouse_report.y >= 2) {
					tap_code(KC_VOLD);
					mediatgl_timer = now;
				}
				//Horizontal movement rewinds and fastforwards
				if (mouse_report.x <= -2) {
					if (os == 1) {
						tap_code16(C(S(KC_LEFT)));
					} else {
						tap_code(KC_MRWD);
					}
					mediatgl_timer = now;
				} else if (mouse_report.x >= 2) {
					if (os == 1) {
						tap_code16(C(S(KC_RGHT)));
					} else {
						tap_code(KC_MFFD);
					}
					mediatgl_timer = now;
				}
			}
			mouse_report.x = 0;
			mouse_report.y = 0;
		//If vertical scroll lock "v_scroll" is on, scrolling is locked to the vertical axis
		} else if (v_scroll) {
			mouse_report.h = 0;
			mouse_report.v = -mouse_report.y;
			mouse_report.x = 0;
			mouse_report.y = 0;
		//If horizontal scroll lock "h_scroll" is on, scrolling is locked to the horizontal axis
		} else if (h_scroll) {
			mouse_report.h = mouse_report.x;
			mouse_report.v = 0;
			mouse_report.x = 0;
			mouse_report.y = 0;
		//If dual/diagonal scroll lock "d_scroll" is on, trackball movement will simultaneously scroll horizontally and vertically, with no constraints
		} else if (d_scroll) {
			mouse_report.h = mouse_report.x;
			mouse_report.v = -(mouse_report.y);
			mouse_report.x = 0;
			mouse_report.y = 0;
		//Default scrolling behavior: trackball movement will scroll horizontally or vertically, with scrolling direction snapped to whichever axis is greater
		//Scrolling slowly diagonally may result in stepped diagonal scrolling; scrolling quickly will result in more snapped behavior
		} else {
			if (abs(mouse_report.x) > abs(mouse_report.y)) {
				mouse_report.h = mouse_report.x;
				mouse_report.v = 0;
			} else {
				mouse_report.v = -mouse_report.y;
				mouse_report.h = 0;
				
			}
			mouse_report.x = 0;
			mouse_report.y = 0;
		}
	}
	//Trackball movement switches to base layer from specified layer
	if (current_layer == 3 && (mouse_report.x != 0 || mouse_report.y != 0)) {
		layer_move(0);
	}
	//Trackball movement does nothing on specified layers
	if (current_layer >= 4) {
		mouse_report.x = 0;
		mouse_report.y = 0;
		mouse_report.h = 0;
		mouse_report.v = 0;
	}
	return mouse_report;
}



//Button layout
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	//Base layer
	[0] = LAYOUT(
	LT(1,KC_BTN2), LYR5CTRL,      LYR3SHFT,      KC_BTN1,
	OSL(7),                                      TT(2)
	),

	//Media/misc layer
	[1] = LAYOUT(
	KC_TRNS,       UNDOPREV,      REDONEXT,      MIDLHOLD,
	KC_MPLY,                                     KC_ENT
	),

	//Alt ball layer
	[2] = LAYOUT(
	SCRLAXIS,      CPI_DOWN,      CPI_UPUP,      ZOOMTGGL,
	MEDIATGL,                                    EXITLYR2
	),

	//Edit layer
	[3] = LAYOUT(
	LT(4,KC_ESC),  BKSPWORD,       DEL_WORD,     TABSHIFT,
	SPC_NBSP,                                    ENTERSFT
	),

	//Arrow layer
	[4] = LAYOUT(
	KC_TRNS,       LEFTHOME,       RIGHTEND,     UPPAGEUP,
	CADQBOOT,                                    DOWNPGDN
	),

	//App/tab switching layer
	[5] = LAYOUT(
	APPBACKX,      TABBACKX,      TABFWRDN,      APPFWRDN,
	OVERVIEW,                                    EXL5MOL6
	),

	//Virtual desktops layer
	[6] = LAYOUT(
	VDESKBKX,      KC_PWR,        G(KC_L),       VDESKFWN,
	OSTOGGLE,                                    KC_TRNS
	),

	//Cut/copy/paste/web layer
	[7] = LAYOUT(
	BACKBACK,      SRCHSRCH,      RFRSRFRS,      FWRDFWRD,
	COPY_CUT,                                    PSTPLAIN
	),

	//Files layer
	[8] = LAYOUT(
	SAVEDNLD,       C(KC_F),       RENAMEFL,      INFOOPEN,
	FILESALL,                                     PRNTSCRN
	),
};
