#include "hmi.h"

#include "main.h"
#include "s7server.h"
#include "leapmotion.h"

extern S7Server *server;
extern volatile t_leap_status leap_status;
extern volatile int retStatus;

volatile bool leap_aktiv = false;

GResource *resources_get_resource(void);

Hmi::Hmi(): pincode("1234")
{
	//Einbindung der Resourcen (Glade UI Datei und Bilder)
	g_resources_register(resources_get_resource());

	app = Gtk::Application::create("");

	//Regelmässiger Aufruf alle 20ms
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &Hmi::timeout_update), 20);

	//Widgets von UI Datei einlesen und verknüpfen
	auto ui = Gtk::Builder::create_from_resource("/hmi/ui.glade");

	ui->get_widget("notebook_main", notebook_main);
	notebook_main->signal_switch_page().connect(sigc::mem_fun(*this, &Hmi::main_switch_page));


	ui->get_widget("button_stopp", button_stopp);
	button_stopp->signal_pressed().connect(sigc::mem_fun(*this, &Hmi::stopp_pressed));
	button_stopp->signal_released().connect(sigc::mem_fun(*this, &Hmi::stopp_released));
	button_stopp->override_background_color(Gdk::RGBA("red"));
	button_stopp->override_background_color(Gdk::RGBA("white"), Gtk::STATE_FLAG_ACTIVE);
	button_stopp->override_color(Gdk::RGBA("white"));
	button_stopp->override_color(Gdk::RGBA("red"), Gtk::STATE_FLAG_ACTIVE);

	ui->get_widget("button_reset", button_reset);
	button_reset->signal_pressed().connect(sigc::mem_fun(*this, &Hmi::reset_pressed));
	button_reset->signal_released().connect(sigc::mem_fun(*this, &Hmi::reset_released));
	button_reset->override_background_color(Gdk::RGBA("white"));
	button_reset->override_color(Gdk::RGBA("blue"));


// Seite "Einrichten"
	ui->get_widget("entry_einrichten_pincode", entry_einrichten_pincode);
	entry_einrichten_pincode->signal_activate().connect(sigc::mem_fun(*this, &Hmi::einrichten_entsperren_clicked));
	ui->get_widget("label_einrichten_pincode_falsch", label_einrichten_pincode_falsch);
	ui->get_widget("button_einrichten_entsperren", button_einrichten_entsperren);
	button_einrichten_entsperren->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::einrichten_entsperren_clicked));
	ui->get_widget("button_einrichten_sperren", button_einrichten_sperren);
	button_einrichten_sperren->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::einrichten_sperren_clicked));
	ui->get_widget("box_einrichten_bedienung", box_einrichten_bedienung);

	ui->get_widget("switch_einrichten_lin_ein", switch_einrichten_lin_ein);
	switch_einrichten_lin_ein->signal_state_flags_changed().connect(sigc::mem_fun(*this, &Hmi::einrichten_lin_ein_state_set));
	ui->get_widget("button_einrichten_lin_reset", button_einrichten_lin_reset);
	button_einrichten_lin_reset->signal_pressed().connect(sigc::mem_fun(*this, &Hmi::einrichten_lin_reset_pressed));
	button_einrichten_lin_reset->signal_released().connect(sigc::mem_fun(*this, &Hmi::einrichten_lin_reset_released));
	ui->get_widget("button_einrichten_lin_grund", button_einrichten_lin_grund);
	button_einrichten_lin_grund->signal_pressed().connect(sigc::mem_fun(*this, &Hmi::einrichten_lin_grund_pressed));
	button_einrichten_lin_grund->signal_released().connect(sigc::mem_fun(*this, &Hmi::einrichten_lin_grund_released));

	ui->get_widget("text_einrichten_lin_status", text_einrichten_lin_status);
	buffer_einrichten_lin_status = text_einrichten_lin_status->get_buffer();

	ui->get_widget("button_einrichten_minimieren", button_einrichten_minimieren);
	button_einrichten_minimieren->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::einrichten_minimieren_clicked));
	ui->get_widget("button_einrichten_schliessen", button_einrichten_schliessen);
	button_einrichten_schliessen->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::einrichten_schliessen_clicked));
	ui->get_widget("button_einrichten_reboot", button_einrichten_reboot);
	button_einrichten_reboot->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::einrichten_reboot_clicked));
	ui->get_widget("button_einrichten_shutdown", button_einrichten_shutdown);
	button_einrichten_shutdown->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::einrichten_shutdown_clicked));

// Seite "Handzeichen"
	ui->get_widget("button_zeichen_faust", button_zeichen_faust);
	button_zeichen_faust->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_faust_clicked));
	ui->get_widget("button_zeichen_peace", button_zeichen_peace);
	button_zeichen_peace->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_peace_clicked));
	ui->get_widget("button_zeichen_daumenhoch", button_zeichen_daumenhoch);
	button_zeichen_daumenhoch->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_daumenhoch_clicked));

	ui->get_widget("button_zeichen_mittelfinger", button_zeichen_mittelfinger);
	button_zeichen_mittelfinger->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_mittelfinger_clicked));

	ui->get_widget("button_zeichen_metal", button_zeichen_metal);
	button_zeichen_metal->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_metal_clicked));

	ui->get_widget("button_zeichen_eigen1", button_zeichen_eigen1);
	button_zeichen_eigen1->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_eigen1_clicked));
	ui->get_widget("button_zeichen_eigen2", button_zeichen_eigen2);
	button_zeichen_eigen2->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_eigen2_clicked));
	ui->get_widget("button_zeichen_eigen3", button_zeichen_eigen3);
	button_zeichen_eigen3->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_eigen3_clicked));
	ui->get_widget("button_zeichen_eigen4", button_zeichen_eigen4);
	button_zeichen_eigen4->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_eigen4_clicked));
	ui->get_widget("button_zeichen_eigen5", button_zeichen_eigen5);
	button_zeichen_eigen5->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_eigen5_clicked));

	ui->get_widget("button_zeichen_grund", button_zeichen_grund);
	button_zeichen_grund->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_grund_clicked));
	ui->get_widget("radio_zeichen_man", radio_zeichen_man);
	ui->get_widget("radio_zeichen_auto", radio_zeichen_auto);
	ui->get_widget("radio_zeichen_endlos", radio_zeichen_endlos);
	adj_zeichen_verz  = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_zeichen_verz"));


// Seite "LeapMotion"
	ui->get_widget("switch_leap_ein", switch_leap_ein);
	switch_leap_ein->signal_state_flags_changed().connect(sigc::mem_fun(*this, &Hmi::leap_ein_state_set));
	ui->get_widget("text_leap_status", text_leap_status);
	buffer_leap_status = text_leap_status->get_buffer();
	leap_status = MISSING;


// Seite "Feineinstellung"
	//Daumen
	arr_adj_fein[0]  = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f0_g0"));
	arr_adj_fein[1]  = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f0_g1"));
	//Zeigefinger
	arr_adj_fein[2]  = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f1_g0"));
	arr_adj_fein[3]  = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f1_g1"));
	arr_adj_fein[4]  = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f1_g2"));
	//Mittelfinger
	arr_adj_fein[5]  = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f2_g0"));
	arr_adj_fein[6]  = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f2_g1"));
	arr_adj_fein[7]  = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f2_g2"));
	//Ringfinger
	arr_adj_fein[8]  = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f3_g0"));
	arr_adj_fein[9]  = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f3_g1"));
	arr_adj_fein[10] = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f3_g2"));
	//Kleiner Finger
	arr_adj_fein[11] = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f4_g0"));
	arr_adj_fein[12] = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f4_g1"));
	arr_adj_fein[13] = Glib::RefPtr<Gtk::Adjustment>::cast_static(ui->get_object("adj_f4_g2"));

	ui->get_widget("button_fein_eigen1", button_fein_eigen1);
	button_fein_eigen1->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::fein_eigen1_clicked));
	ui->get_widget("button_fein_eigen2", button_fein_eigen2);
	button_fein_eigen2->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::fein_eigen2_clicked));
	ui->get_widget("button_fein_eigen3", button_fein_eigen3);
	button_fein_eigen3->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::fein_eigen3_clicked));
	ui->get_widget("button_fein_eigen4", button_fein_eigen4);
	button_fein_eigen4->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::fein_eigen4_clicked));
	ui->get_widget("button_fein_eigen5", button_fein_eigen5);
	button_fein_eigen5->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::fein_eigen5_clicked));


	ui->get_widget("window_main", pWindow);
	pWindow->fullscreen();
	pWindow->show();
}


bool Hmi::run() {
	return app->run(*pWindow);
}

void Hmi::main_switch_page(Gtk::Widget* page, guint page_number) {
	//Wird bei Seitenwechsel aufgerufen, laufende Funktionen deaktivieren
	timeout_zeichen_conn.disconnect();
	leap_aktiv = false;
	switch_leap_ein->set_state(false);
}

void Hmi::stopp_pressed() {
	server->DB_Send.s.stopp = true;
}

void Hmi::stopp_released() {
	server->DB_Send.s.stopp = false;
	button_reset->set_sensitive(true);
	button_reset->override_background_color(Gdk::RGBA("blue"));
	button_reset->override_background_color(Gdk::RGBA("white"), Gtk::STATE_FLAG_ACTIVE);
	button_reset->override_color(Gdk::RGBA("white"));
	button_reset->override_color(Gdk::RGBA("blue"), Gtk::STATE_FLAG_ACTIVE);
}

void Hmi::reset_pressed() {
	server->DB_Send.s.reset = true;
	setWinkel(zeichen_grund);
}

void Hmi::reset_released() {
	server->DB_Send.s.reset = false;
	button_reset->set_sensitive(false);
	button_reset->override_background_color(Gdk::RGBA("white"));
	button_reset->override_color(Gdk::RGBA("blue"));

}

bool Hmi::timeout_update() {
	//Wird alle 20ms aufgerufen, seitenspezifische Update-Funktionen
	int page = notebook_main->get_current_page();
	switch (page) {
		case 1:
			update_lin_status();
			break;
		case 2:
			timeout_zeichen_value = adj_zeichen_verz->get_value() * 1000;
			break;
		case 3:
			update_leap_status();
			break;
		case 4:
			update_fein();
			break;
		default:
			break;
	}
	return true;
}


// Seite "Einrichten"
void Hmi::einrichten_entsperren_clicked() {
	if (!entry_einrichten_pincode->get_text().compare(pincode)) {
		//Pincode ist richtig
		box_einrichten_bedienung->set_sensitive(true);
		label_einrichten_pincode_falsch->set_visible(false);
		entry_einrichten_pincode->delete_text(0, -1);
		button_einrichten_sperren->set_sensitive(true);
		button_einrichten_entsperren->set_sensitive(false);
		entry_einrichten_pincode->set_sensitive(false);
	} else {
		//Pincode ist falsch
		entry_einrichten_pincode->delete_text(0, -1);
		label_einrichten_pincode_falsch->set_visible(true);
	}
}

void Hmi::einrichten_sperren_clicked() {
	box_einrichten_bedienung->set_sensitive(false);
	button_einrichten_sperren->set_sensitive(false);
	button_einrichten_entsperren->set_sensitive(true);
	entry_einrichten_pincode->set_sensitive(true);
}

void Hmi::einrichten_lin_ein_state_set(Gtk::StateFlags previous_state_flags) {
	server->DB_Send.s.lin_ein = switch_einrichten_lin_ein->get_state();
}

void Hmi::einrichten_lin_reset_pressed() {
	server->DB_Send.s.lin_reset = true;
}

void Hmi::einrichten_lin_reset_released() {
	server->DB_Send.s.lin_reset = false;
}

void Hmi::einrichten_lin_grund_pressed() {
	server->DB_Send.s.lin_grundstellung = true;
	setWinkel(zeichen_grund);
}

void Hmi::einrichten_lin_grund_released() {
	server->DB_Send.s.lin_grundstellung = false;
}

void Hmi::einrichten_minimieren_clicked() {
	pWindow->iconify();
}

//Je nach Fehlerstatus reagiert das Startscript anders
void Hmi::einrichten_schliessen_clicked() {
	retStatus = 50;
	pWindow->close();
}

void Hmi::einrichten_reboot_clicked() {
	retStatus = 51;
	pWindow->close();
}

void Hmi::einrichten_shutdown_clicked() {
	retStatus = 52;
	pWindow->close();
}

void Hmi::update_lin_status() {
	server->LockArea(srvAreaDB, 2);
	int status = server->DB_Recv.s.lin_status;
	server->UnlockArea(srvAreaDB, 2);

	//Die Anzeige wird nur aktualisiert, wenn sich der Status verändert hat
	if (status != einrichten_lin_prev_status) {
		if (status & 0b0011111111111111) {
			text_einrichten_lin_status->override_color(Gdk::RGBA("red"));
			buffer_einrichten_lin_status->set_text(Glib::ustring("Folgende Lineareinheiten melden einen Fehler: "));

			//Lineareinheiten mit Fehler auflisten
			for (int i = 0; i < 14; i++) {
				if (server->DB_Recv.s.lin_status & (1 << i)) {
					buffer_einrichten_lin_status->insert_at_cursor(Glib::ustring("\n"));
					buffer_einrichten_lin_status->insert_at_cursor(Glib::ustring::format(i + 1));
				}
			}
		} else {
			buffer_einrichten_lin_status->set_text(Glib::ustring("Keine Fehlermeldungen."));
			text_einrichten_lin_status->unset_color();
		}
	}
	einrichten_lin_prev_status = status;
}


// Seite "Handzeichen"
void Hmi::zeichen_anfahren(unsigned char* zeichen) {
	zeichen_aktuell = zeichen;
	setWinkel(zeichen_aktuell);
	if (!timeout_zeichen_conn.connected()) {
		if (radio_zeichen_auto->get_active()) {
			timeout_zeichen_conn = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Hmi::timeout_zeichen_auto_grund), timeout_zeichen_value);
		} else if (radio_zeichen_endlos->get_active()){
			timeout_zeichen_conn = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Hmi::timeout_zeichen_endlos_grund), timeout_zeichen_value);
		}
	}
}

void Hmi::zeichen_faust_clicked() {
	zeichen_anfahren(zeichen_faust);
}

void Hmi::zeichen_peace_clicked() {
	zeichen_anfahren(zeichen_peace);
}

void Hmi::zeichen_daumenhoch_clicked() {
	zeichen_anfahren(zeichen_daumenhoch);
}

void Hmi::zeichen_mittelfinger_clicked() {
	zeichen_anfahren(zeichen_mittelfinger);
}

void Hmi::zeichen_metal_clicked() {
	zeichen_anfahren(zeichen_metal);
}

void Hmi::zeichen_grund_clicked() {
	setWinkel(zeichen_grund);
	timeout_zeichen_conn.disconnect();
}

void Hmi::zeichen_eigen1_clicked() {
	zeichen_anfahren(zeichen1);
}

void Hmi::zeichen_eigen2_clicked() {
	zeichen_anfahren(zeichen2);
}

void Hmi::zeichen_eigen3_clicked() {
	zeichen_anfahren(zeichen3);
}

void Hmi::zeichen_eigen4_clicked() {
	zeichen_anfahren(zeichen4);
}

void Hmi::zeichen_eigen5_clicked() {
	zeichen_anfahren(zeichen5);
}

bool Hmi::timeout_zeichen_auto_grund() {
	setWinkel(zeichen_grund);
	return false;
}

bool Hmi::timeout_zeichen_endlos_fahren() {
	if (radio_zeichen_endlos->get_active()) {
		setWinkel(zeichen_aktuell);
		timeout_zeichen_conn = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Hmi::timeout_zeichen_endlos_grund), timeout_zeichen_value);
	}
	return false;
}

bool Hmi::timeout_zeichen_endlos_grund() {
	setWinkel(zeichen_grund);
	printf("endlos grund\n");
	timeout_zeichen_conn = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Hmi::timeout_zeichen_endlos_fahren), timeout_zeichen_value);
	return false;
}


// Seite "LeapMotion"
void Hmi::leap_ein_state_set(Gtk::StateFlags previous_state_flags) {
	leap_aktiv = switch_leap_ein->get_state();
}

void Hmi::update_leap_status() {
	switch (leap_status) {
		case MISSING:
			buffer_leap_status->set_text(Glib::ustring("Es liegt ein Problem mit dem LeapMotion-Controller vor"));
			break;
		case CONNECTED:
			buffer_leap_status->set_text(Glib::ustring("Der LeapMotion-Controller wurde verbunden"));
			break;
		case NOHAND:
			buffer_leap_status->set_text(Glib::ustring("Keine Hand im Blickfeld erkannt"));
			break;
		case TRACKING:
			if (switch_leap_ein->get_state()) {
				buffer_leap_status->set_text(Glib::ustring("Die Handerfassung läuft"));
			} else {
				buffer_leap_status->set_text(Glib::ustring("Die Handerfassung ist bereit"));
			}
			break;
		case TOOMANYHANDS:
			buffer_leap_status->set_text(Glib::ustring("Es darf sich nur eine Hand im Blickfeld befinden!"));
			break;
		default:
			break;
	}
}


// Seite "Feinsteuerung"
void Hmi::update_fein() {
	unsigned char winkel[14];
	for (int i = 0; i < 14; i++) {
		winkel[i] = arr_adj_fein[i]->get_value();
	}
	setWinkel(winkel);
}

void Hmi::fein_eigen1_clicked() {
	fein_speichern(zeichen1);
}

void Hmi::fein_eigen2_clicked() {
	fein_speichern(zeichen2);
}

void Hmi::fein_eigen3_clicked() {
	fein_speichern(zeichen3);
}

void Hmi::fein_eigen4_clicked() {
	fein_speichern(zeichen4);
}

void Hmi::fein_eigen5_clicked() {
	fein_speichern(zeichen5);
}

void Hmi::fein_speichern(unsigned char *zeichen) {
	for (int i = 0; i < 14; i++) {
		zeichen[i] = arr_adj_fein[i]->get_value();
	}
}
