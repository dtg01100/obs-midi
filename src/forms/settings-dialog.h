/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/
#pragma once
#include <QtWidgets/QDialog>
#include <vector>

#include "ui_settings-dialog.h"
#include "../midi-agent.h"
#include "../version.h"
#include "../Midi_hook.h"
class PluginWindow : public QDialog {
	Q_OBJECT
public:
	explicit PluginWindow(QWidget *parent);
	~PluginWindow() override;
public slots:
	void ToggleShowHide();
private slots:
	void handle_check_enabled_state_changed(int state);
	void handle_device_select(const QString &curitem);
	// Auto-connect friendly aliases (match UI signal names)
	void on_check_enabled_stateChanged(int state);
	void on_list_midi_dev_currentRowChanged(int currentRow);
	void handle_midi_message(const MidiMessage &mess);
	void obs_actions_select(const QString &action);
	void set_edit_mode();
	void save_edit();

	void reset_to_defaults();
	void clear_actions_box(QLayout *layout);
	void set_all_cell_colors(int row);
	void tab_changed(int tab);
	void clear_table();
	void remove_hook(MidiMapping *hook);
	void delete_mapping();
	void add_new_mapping();
	void edit_mapping();
	//static void set_cell_colors(const QColor &color, QTableWidgetItem *item);

private:
	Ui::PluginWindow *ui;
	bool hidedebugitems = true;
	bool loadingdevices = false;
	bool starting = true;
	bool listening = false;
	//void configure_table() const;
	void set_title_window();
	void connect_ui_signals();
	void setup_actions();
	//	void setCheck(bool check); // not implemented
	void load_devices();

	//static QStringList translatelist(QStringList list);
	
	void add_midi_device(const QString &Name);
	//void set_headers() const;
	void set_configure_title(const QString &title);
	void disconnect_midi_message_handler();
	void connect_midi_message_handler();
	bool map_exists() const;
	bool verify_mapping() const;
	void add_row_from_hook(const MidiMapping *hook);
	void load_table();
	void table_select(int selection);
	void midi_message_select(MidiMapping *hook);
private:
	int editrow = -1;
	bool editmode = false;
	bool switching = false;
	MidiMapping *edithook;
};
