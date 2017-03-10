#ifndef HMI_H
#define HMI_H

#include <gtkmm.h>

class Hmi
{
	public:
		Hmi();
//		virtual ~Hmi();
		bool run();


		void stopp_clicked();
		void reset_clicked();
		Gtk::Button *button_stopp = nullptr;
		Gtk::Button *button_reset = nullptr;

		// Seite "Einrichten"
		Glib::ustring pincode;

		void einrichten_entsperren_clicked();
		void einrichten_sperren_clicked();

		void einrichten_lin_ein_state_set(Gtk::StateFlags);
		void einrichten_lin_reset_pressed();
		void einrichten_lin_reset_released();
		void einrichten_lin_grund_pressed();
		void einrichten_lin_grund_released();

		Gtk::Entry  *entry_einrichten_pincode = nullptr;
		Gtk::Label  *label_einrichten_pincode_falsch = nullptr;
		Gtk::Button *button_einrichten_entsperren = nullptr;
		Gtk::Button *button_einrichten_sperren = nullptr;
		Gtk::Box    *box_einrichten_bedienung = nullptr;
		Gtk::Switch *switch_einrichten_lin_ein = nullptr;
		Gtk::Button *button_einrichten_lin_reset = nullptr;
		Gtk::Button *button_einrichten_lin_grund = nullptr;


		// Seite "Handzeichen"
		void zeichen_faust_clicked();
		void zeichen_peace_clicked();
		void zeichen_daumenhoch_clicked();

		void zeichen_grund_clicked();
//		void zeichen_man_state_set(Gtk::StateFlags);
//		void zeichen_endlos_state_set(Gtk::StateFlags);

		Gtk::Button      *button_zeichen_faust = nullptr;
		Gtk::Button      *button_zeichen_peace = nullptr;
		Gtk::Button      *button_zeichen_daumenhoch = nullptr;

		Gtk::Button      *button_zeichen_grund = nullptr;
		Gtk::RadioButton *radio_zeichen_man = nullptr;
		Gtk::RadioButton *radio_zeichen_auto = nullptr;
		Gtk::RadioButton *radio_zeichen_endlos = nullptr;

		void zeichen_anfahren(unsigned char* zeichen);
		unsigned char* zeichen_aktuell;
		sigc::connection timeout_zeichen_conn;
		bool timeout_zeichen_auto_grund();
		bool timeout_zeichen_endlos_fahren();
		bool timeout_zeichen_endlos_grund();
		const int timeout_zeichen_value = 3000;


		// Seite "LeapMotion"
		void leap_ein_state_set(Gtk::StateFlags);

		Gtk::Switch     *switch_leap_ein = nullptr;
		Gtk::TextView   *text_leap_status = nullptr;
		Glib::RefPtr<Gtk::TextBuffer> buffer_leap_status;

		bool timeout_update();

		Glib::RefPtr<Gtk::Builder> m_refBuilder;

		Gtk::Window* pWindow;
		Glib::RefPtr<Gtk::Application> app;
};

#endif
