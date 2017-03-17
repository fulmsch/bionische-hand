#include "hmi.h"

#include "main.h"
#include "s7server.h"
#include "leapmotion.h"

extern S7Server *server;
extern volatile t_leap_status leap_status;

volatile bool leap_aktiv = false;

GResource *resources_get_resource(void);

Hmi::Hmi(): pincode("1234")
{
	g_resources_register(resources_get_resource());

//	app = new Gtk::Main;
	
	app = Gtk::Application::create("");

	auto ui = Gtk::Builder::create_from_resource("/hmi/ui.glade");

	Glib::signal_timeout().connect(sigc::mem_fun(*this, &Hmi::timeout_update), 20);

	ui->get_widget("notebook_main", notebook_main);
	notebook_main->signal_switch_page().connect(sigc::mem_fun(*this, &Hmi::main_switch_page));


	ui->get_widget("button_stopp", button_stopp);
	button_stopp->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::stopp_clicked));
	button_stopp->override_background_color(Gdk::RGBA("red"));
	button_stopp->override_background_color(Gdk::RGBA("white"), Gtk::STATE_FLAG_ACTIVE);
	button_stopp->override_color(Gdk::RGBA("white"));
	button_stopp->override_color(Gdk::RGBA("red"), Gtk::STATE_FLAG_ACTIVE);

	ui->get_widget("button_reset", button_reset);
	button_reset->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::reset_clicked));
	button_reset->override_background_color(Gdk::RGBA("white"));
	button_reset->override_color(Gdk::RGBA("blue"));

// Seite "Start"
//	ui->get_widget("fixed_start", fixed_start);

// Seite "Einrichten"
//	ui->get_widget("box_einrichten", box_einrichten);

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


// Seite "Handzeichen"
//	ui->get_widget("box_zeichen", box_zeichen);

	ui->get_widget("button_zeichen_faust", button_zeichen_faust);
	button_zeichen_faust->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_faust_clicked));
	ui->get_widget("button_zeichen_peace", button_zeichen_peace);
	button_zeichen_peace->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_peace_clicked));
	ui->get_widget("button_zeichen_daumenhoch", button_zeichen_daumenhoch);
	button_zeichen_daumenhoch->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_daumenhoch_clicked));

	ui->get_widget("button_zeichen_grund", button_zeichen_grund);
	button_zeichen_grund->signal_clicked().connect(sigc::mem_fun(*this, &Hmi::zeichen_grund_clicked));
	ui->get_widget("radio_zeichen_man", radio_zeichen_man);
	ui->get_widget("radio_zeichen_auto", radio_zeichen_auto);
//	switch_zeichen_man->signal_state_flags_changed().connect(sigc::mem_fun(*this, &Hmi::zeichen_man_state_set));
	ui->get_widget("radio_zeichen_endlos", radio_zeichen_endlos);
//	switch_zeichen_endlos->signal_state_flags_changed().connect(sigc::mem_fun(*this, &Hmi::zeichen_endlos_state_set));


// Seite "LeapMotion"
//	ui->get_widget("box_leap", box_leap);

	ui->get_widget("switch_leap_ein", switch_leap_ein);
	switch_leap_ein->signal_state_flags_changed().connect(sigc::mem_fun(*this, &Hmi::leap_ein_state_set));
	ui->get_widget("text_leap_status", text_leap_status);
	buffer_leap_status = text_leap_status->get_buffer();
	leap_status = MISSING;


	ui->get_widget("window_main", pWindow);
	pWindow->show();
}

//Hmi::~Hmi() {
//
//}

void Hmi::main_switch_page(Gtk::Widget* page, guint page_number) {
//	printf("seite %d\n", page_number);
	timeout_zeichen_conn.disconnect();
	leap_aktiv = false;
	switch_leap_ein->set_state(false);
//	switch (page_number) {
//		case 0:
//			printf("start\n");
//			break;
//		case 1:
//			printf("einrichten\n");
//			break;
//		case 2:
//			printf("zeichen\n");
//			break;
//		case 3:
//			printf("leap\n");
//			break;
//		default:
//			break;
//	}

}

void Hmi::stopp_clicked() {
	button_reset->set_sensitive(true);
	button_reset->override_background_color(Gdk::RGBA("blue"));
	button_reset->override_background_color(Gdk::RGBA("white"), Gtk::STATE_FLAG_ACTIVE);
	button_reset->override_color(Gdk::RGBA("white"));
	button_reset->override_color(Gdk::RGBA("blue"), Gtk::STATE_FLAG_ACTIVE);
}

void Hmi::reset_clicked() {
	button_reset->set_sensitive(false);
	button_reset->override_background_color(Gdk::RGBA("white"));
	button_reset->override_color(Gdk::RGBA("blue"));

}

void Hmi::einrichten_entsperren_clicked() {
	if (!entry_einrichten_pincode->get_text().compare(pincode)) {
		box_einrichten_bedienung->set_sensitive(true);
		label_einrichten_pincode_falsch->set_visible(false);
		entry_einrichten_pincode->delete_text(0, -1);
		button_einrichten_sperren->set_sensitive(true);
		button_einrichten_entsperren->set_sensitive(false);
		entry_einrichten_pincode->set_sensitive(false);
	} else {
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
	printf("%x\n", server->DB_Send.a[15]);
}

void Hmi::einrichten_lin_reset_pressed() {
	server->DB_Send.s.lin_reset = true;
	printf("%x\n", server->DB_Send.a[15]);
}

void Hmi::einrichten_lin_reset_released() {
	server->DB_Send.s.lin_reset = false;
	printf("%x\n", server->DB_Send.a[15]);
}

void Hmi::einrichten_lin_grund_pressed() {
	server->DB_Send.s.lin_grundstellung = true;
	setAngles(zeichen_grund);
	printf("%x\n", server->DB_Send.a[15]);
}

void Hmi::einrichten_lin_grund_released() {
	server->DB_Send.s.lin_grundstellung = false;
	printf("%x\n", server->DB_Send.a[15]);
}

void Hmi::zeichen_anfahren(unsigned char* zeichen) {
	zeichen_aktuell = zeichen;
	setAngles(zeichen_aktuell);
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

void Hmi::zeichen_grund_clicked() {
	setAngles(zeichen_grund);
	timeout_zeichen_conn.disconnect();
}

bool Hmi::timeout_zeichen_auto_grund() {
	setAngles(zeichen_grund);
	printf("auto grund\n");
	return false;
}

bool Hmi::timeout_zeichen_endlos_fahren() {
	if (radio_zeichen_endlos->get_active()) {
		setAngles(zeichen_aktuell);
		printf("endlos fahren\n");
		timeout_zeichen_conn = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Hmi::timeout_zeichen_endlos_grund), timeout_zeichen_value);
	}
	return false;
}

bool Hmi::timeout_zeichen_endlos_grund() {
	setAngles(zeichen_grund);
	printf("endlos grund\n");
	timeout_zeichen_conn = Glib::signal_timeout().connect(sigc::mem_fun(*this, &Hmi::timeout_zeichen_endlos_fahren), timeout_zeichen_value);
	return false;
}

void Hmi::leap_ein_state_set(Gtk::StateFlags previous_state_flags) {
	leap_aktiv = switch_leap_ein->get_state();
}

bool Hmi::timeout_update() {
	update_leap_status();
	update_lin_status();
	return true;
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

void Hmi::update_lin_status() {
	server->LockArea(srvAreaDB, 2);
	if (server->DB_Recv.s.lin_status & 0b0011111111111111) {
		text_einrichten_lin_status->override_color(Gdk::RGBA("red"));
		buffer_einrichten_lin_status->set_text(Glib::ustring("Folgende Lineareinheiten melden einen Fehler: "));
		for (int i = 0; i < 14; i++) {
			if (server->DB_Recv.s.lin_status & (1 << i)) {
				buffer_einrichten_lin_status->insert_at_cursor(Glib::ustring("\n"));
				buffer_einrichten_lin_status->insert_at_cursor(Glib::ustring::format(i + 1));
			}
		}
//		buffer_einrichten_lin_status->insert_at_cursor(Glib::ustring("\b\b."));
	} else {
		buffer_einrichten_lin_status->set_text(Glib::ustring("Keine Fehlermeldungen."));
		text_einrichten_lin_status->unset_color();
	}
	server->UnlockArea(srvAreaDB, 2);
}

bool Hmi::run() {
//	return app->iteration(true);
	return app->run(*pWindow);
}
