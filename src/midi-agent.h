/*
obs-midi

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

#if __has_include(<obs-frontend-api.h>)
#include <obs-frontend-api.h>
#include "rtmidi17/rtmidi17.hpp"
#else
#include <obs-frontend-api/obs-frontend-api.h>
#include "rtmidi17/rtmidi17.hpp"
#endif
#include <QtCore/QString>
#include <QtCore/QSharedPointer>
#include <vector>
#include <QObject>
#include <functional>
#include <map>
#include <string>
#include <iostream>
#include "obs-controller.h"
#include "obs-controller.h"
#include "rpc/RpcEvent.h"
#include "utils.h"
using namespace std;

class MidiHook {
public:
	string type;
	int index;
	bool bidirectional;
	int mchan; //midi channel
	string action;
	string command;
	string param1;
	string param2;
	string param3;
	MidiHook(){};
	MidiHook(string midiMessageType, int midiChannelIndex, int midiChannel,
		 bool bidirectionals, string OBSCommand, string p1 = "",
		 string p2 = "", string p3 = "", string actionType = "")
		: type(midiMessageType),
		  index(midiChannelIndex),
		  mchan(midiChannel),
		  bidirectional(bidirectionals),
		  command(OBSCommand),
		  param1(p1),
		  param2(p2),
		  param3(p3),
		  action(actionType)
	{
		// if action not provided, default to button or fader depending on command
		if (actionType.empty()) {
			action = (command == "note_on" ? "button" : "fader");
		}
	}

	MidiHook(const char *jsonString)
	{
		obs_data_t *data = obs_data_create_from_json(jsonString);
		type = obs_data_get_string(data, "type");
		index = obs_data_get_int(data, "index");
		mchan = obs_data_get_int(data, "mchan");
		action = obs_data_get_string(data, "action");
		command = obs_data_get_string(data, "command");
		param1 = obs_data_get_string(data, "param1");
		param2 = obs_data_get_string(data, "param2");
		param3 = obs_data_get_string(data, "param3");
	}

	obs_data_t *GetData()
	{
		obs_data_t *data = obs_data_create();
		obs_data_set_string(data, "type", type.c_str());
		obs_data_set_int(data, "index", index);
		obs_data_set_int(data, "mchan", mchan);
		obs_data_set_string(data, "action", action.c_str());
		obs_data_set_string(data, "command", command.c_str());
		obs_data_set_string(data, "param1", param1.c_str());
		obs_data_set_string(data, "param2", param2.c_str());
		obs_data_set_string(data, "param3", param3.c_str());
		return data;
	}

	const char *ToJSON() { return obs_data_get_json(GetData()); }
};

class MidiAgent : public QObject {
	Q_OBJECT

public:
	MidiAgent();
	~MidiAgent();
	void Load(obs_data_t *data);

	void OpenPort(int inport);
	void OpenOutPort(int outport);
	void ClosePort();

	string GetName();
	string GetOutName();
	void SetOutName(string oname);
	int GetPort();
	bool isEnabled();
	bool isConnected();
	bool isBidirectional();
	bool setBidirectional(bool state);
	static void HandleInput(const rtmidi::message &message, void *userData);
	void TriggerInputCommand(MidiHook *hook, int midiVal);
	void SendMessage(std::string names, std::string mType, int mIndex,
			 int channel);
	vector<MidiHook *> GetMidiHooks();
	void AddMidiHook(MidiHook *hook);
	void RemoveMidiHook(MidiHook *hook);
	void ClearMidiHooks();
	obs_data_t *GetData();

public slots:
	void NewObsEvent(QString eventType, QString eventData);
signals:
	void SendNewUnknownMessage(QString names, QString mtype, int msgindex,
				   int channel);

private:
	void send(string type, int channel, int norc, int value = 0);
	rtmidi::midi_in *midiin;
	rtmidi::midi_out *midiout;
	string name;
	string outname;
	bool sending;
	int port;
	int lastscenebtn;
	bool enabled;
	bool connected;
	bool bidirectional;
	bool closing = false;
	vector<MidiHook *> midiHooks;
};
