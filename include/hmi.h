#ifndef HMI_H
#define HMI_H

#include <gtkmm.h>

class Hmi
{
	public:
		Hmi();
//		virtual ~Hmi();
		bool run();

	private:
		bool timeout_update();

		void update_leap_status();
		void update_lin_status();

		Gtk::Notebook *notebook_main = nullptr;
		void main_switch_page(Gtk::Widget* page, guint page_number);


		void stopp_pressed();
		void stopp_released();
		void reset_pressed();
		void reset_released();
		Gtk::Button *button_stopp = nullptr;
		Gtk::Button *button_reset = nullptr;

		// Seite "Start"
//		Gtk::Fixed *fixed_start = nullptr;


		// Seite "Einrichten"
//		Gtk::Box *box_einrichten = nullptr;

		Glib::ustring pincode;

		void einrichten_entsperren_clicked();
		void einrichten_sperren_clicked();

		void einrichten_lin_ein_state_set(Gtk::StateFlags);
		void einrichten_lin_reset_pressed();
		void einrichten_lin_reset_released();
		void einrichten_lin_grund_pressed();
		void einrichten_lin_grund_released();

		void einrichten_minimieren_clicked();
		void einrichten_schliessen_clicked();
		void einrichten_reboot_clicked();
		void einrichten_shutdown_clicked();

		Gtk::Entry  *entry_einrichten_pincode = nullptr;
		Gtk::Label  *label_einrichten_pincode_falsch = nullptr;
		Gtk::Button *button_einrichten_entsperren = nullptr;
		Gtk::Button *button_einrichten_sperren = nullptr;
		Gtk::Box    *box_einrichten_bedienung = nullptr;
		Gtk::Switch *switch_einrichten_lin_ein = nullptr;
		Gtk::Button *button_einrichten_lin_reset = nullptr;
		Gtk::Button *button_einrichten_lin_grund = nullptr;

		Gtk::TextView   *text_einrichten_lin_status = nullptr;
		Glib::RefPtr<Gtk::TextBuffer> buffer_einrichten_lin_status;

		Gtk::Button *button_einrichten_minimieren = nullptr;
		Gtk::Button *button_einrichten_schliessen = nullptr;
		Gtk::Button *button_einrichten_reboot = nullptr;
		Gtk::Button *button_einrichten_shutdown = nullptr;


		// Seite "Handzeichen"
//		Gtk::Box *box_zeichen = nullptr;

		void zeichen_faust_clicked();
		void zeichen_peace_clicked();
		void zeichen_daumenhoch_clicked();
		void zeichen_mittelfinger_clicked();
		void zeichen_metal_clicked();

		void zeichen_eigen1_clicked();
		void zeichen_eigen2_clicked();
		void zeichen_eigen3_clicked();
		void zeichen_eigen4_clicked();
		void zeichen_eigen5_clicked();

		void zeichen_grund_clicked();
//		void zeichen_man_state_set(Gtk::StateFlags);
//		void zeichen_endlos_state_set(Gtk::StateFlags);

		Gtk::Button      *button_zeichen_faust = nullptr;
		Gtk::Button      *button_zeichen_peace = nullptr;
		Gtk::Button      *button_zeichen_daumenhoch = nullptr;
		Gtk::Button      *button_zeichen_mittelfinger = nullptr;
		Gtk::Button      *button_zeichen_metal = nullptr;

		Gtk::Button      *button_zeichen_eigen1 = nullptr;
		Gtk::Button      *button_zeichen_eigen2 = nullptr;
		Gtk::Button      *button_zeichen_eigen3 = nullptr;
		Gtk::Button      *button_zeichen_eigen4 = nullptr;
		Gtk::Button      *button_zeichen_eigen5 = nullptr;

		Gtk::Button      *button_zeichen_grund = nullptr;
		Gtk::RadioButton *radio_zeichen_man = nullptr;
		Gtk::RadioButton *radio_zeichen_auto = nullptr;
		Gtk::RadioButton *radio_zeichen_endlos = nullptr;

		Glib::RefPtr<Gtk::Adjustment> adj_zeichen_verz;

		void zeichen_anfahren(unsigned char* zeichen);
		unsigned char* zeichen_aktuell;
		sigc::connection timeout_zeichen_conn;
		bool timeout_zeichen_auto_grund();
		bool timeout_zeichen_endlos_fahren();
		bool timeout_zeichen_endlos_grund();
		int timeout_zeichen_value = 3000;


		// Seite "LeapMotion"
//		Gtk::Box *box_leap = nullptr;

		void leap_ein_state_set(Gtk::StateFlags);

		Gtk::Switch     *switch_leap_ein = nullptr;
		Gtk::TextView   *text_leap_status = nullptr;
		Glib::RefPtr<Gtk::TextBuffer> buffer_leap_status;


		// Seite "Feineinstellung"
		Glib::RefPtr<Gtk::Adjustment> arr_adj_fein[14];
		void update_fein();
		void fein_speichern(unsigned char *);

		void fein_eigen1_clicked();
		void fein_eigen2_clicked();
		void fein_eigen3_clicked();
		void fein_eigen4_clicked();
		void fein_eigen5_clicked();

		Gtk::Button *button_fein_eigen1 = nullptr;
		Gtk::Button *button_fein_eigen2 = nullptr;
		Gtk::Button *button_fein_eigen3 = nullptr;
		Gtk::Button *button_fein_eigen4 = nullptr;
		Gtk::Button *button_fein_eigen5 = nullptr;


		Glib::RefPtr<Gtk::Builder> m_refBuilder;

		Gtk::Window* pWindow;
		Glib::RefPtr<Gtk::Application> app;
};

#endif
