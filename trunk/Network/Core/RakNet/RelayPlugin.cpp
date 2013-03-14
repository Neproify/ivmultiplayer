/*
#include "NativeFeatureIncludes.h"
#if _RAKNET_SUPPORT_RelayPlugin==1

#include "RelayPlugin.h"
#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "BitStream.h"

using namespace RakNet;

enum RelayPluginEnums
{
	TO_RELAY,
	FROM_RELAY,
	ADD_CLIENT,
	ADD_CLIENT_RESPONSE,
	REMOVE_CLIENT,
	USER_ENTERED_ROOM,
	USER_LEFT_ROOM,
	ROOM_CHAT_MSG,
	CHAT_ROOM_LIST
};

STATIC_FACTORY_DEFINITIONS(RelayPlugin,RelayPlugin);

RelayPlugin::RelayPlugin()
{
	acceptAddParticipantRequests=false;
}

RelayPlugin::~RelayPlugin()
{
	DataStructures::List<StrAndGuidAndRoom*> itemList;
	DataStructures::List<RakString> keyList;
	strToGuidHash.GetAsList(itemList, keyList, _FILE_AND_LINE_);
	guidToStrHash.Clear(_FILE_AND_LINE_);
	for (unsigned int i=0; i < itemList.Size(); i++)
		RakNet::OP_DELETE(itemList[i], _FILE_AND_LINE_);
	for (unsigned int i=0; i < chatRooms.Size(); i++)
		RakNet::OP_DELETE(chatRooms[i], _FILE_AND_LINE_);
}

bool RelayPlugin::AddParticipantOnServer(const RakString &key, const RakNetGUID &guid)
{
	ConnectionState cs = rakPeerInterface->GetConnectionState(guid);
	if (cs!=IS_CONNECTED)
		return false;

	if (strToGuidHash.HasData(key)==true)
		return false; // Name already in use

	// If GUID is already in use, remove existing
	StrAndGuidAndRoom *strAndGuidExisting;
	if (guidToStrHash.Pop(strAndGuidExisting, guid, _FILE_AND_LINE_))
	{
		strToGuidHash.Remove(strAndGuidExisting->str, _FILE_AND_LINE_);
		RakNet::OP_DELETE(strAndGuidExisting, _FILE_AND_LINE_);
	}

	StrAndGuidAndRoom *strAndGuid = RakNet::OP_NEW<StrAndGuidAndRoom>(_FILE_AND_LINE_);
	strAndGuid->guid=guid;
	strAndGuid->str=key;

	strToGuidHash.Push(key, strAndGuid, _FILE_AND_LINE_);
	guidToStrHash.Push(guid, strAndGuid, _FILE_AND_LINE_);

	return true;
}
void RelayPlugin::RemoveParticipantOnServer(const RakNetGUID &guid)
{
	StrAndGuidAndRoom *strAndGuid;
	if (guidToStrHash.Pop(strAndGuid, guid, _FILE_AND_LINE_))
	{
		LeaveRoom(&strAndGuid);
		strToGuidHash.Remove(strAndGuid->str, _FILE_AND_LINE_);
		RakNet::OP_DELETE(strAndGuid, _FILE_AND_LINE_);
	}
}

void RelayPlugin::SetAcceptAddParticipantRequests(bool accept)
{
	acceptAddParticipantRequests=accept;
}
void RelayPlugin::AddParticipantRequestFromClient(const RakString &key, const RakNetGUID &relayPluginServerGuid)
{
	BitStream bsOut;
	bsOut.WriteCasted<MessageID>(ID_RELAY_PLUGIN);
	bsOut.WriteCasted<MessageID>(ADD_CLIENT);
	bsOut.WriteCompressed(key);
	SendUnified(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, relayPluginServerGuid, false);
}
void RelayPlugin::RemoveParticipantRequestFromClient(const RakNetGUID &relayPluginServerGuid)
{
	BitStream bsOut;
	bsOut.WriteCasted<MessageID>(ID_RELAY_PLUGIN);
	bsOut.WriteCasted<MessageID>(REMOVE_CLIENT);
	SendUnified(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, relayPluginServerGuid, false);
}
// Send a message to a server running RelayPlugin, to forward a message to the system identified by \a key
void RelayPlugin::SendToParticipant(const RakNetGUID &relayPluginServerGuid, const RakString &key, BitStream *bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel)
{
	BitStream bsOut;
	bsOut.WriteCasted<MessageID>(ID_RELAY_PLUGIN);
	bsOut.WriteCasted<MessageID>(TO_RELAY);
	bsOut.WriteCasted<unsigned char>(priority);
	bsOut.WriteCasted<unsigned char>(reliability);
	bsOut.Write(orderingChannel);
	bsOut.WriteCompressed(key);
	bsOut.Write(bitStream);
	SendUnified(&bsOut, priority, reliability, orderingChannel, relayPluginServerGuid, false);
}

PluginReceiveResult RelayPlugin::OnReceive(Packet *packet)
{
	if (packet->data[0]==ID_RELAY_PLUGIN)
	{
		switch (packet->data[1])
		{
		case TO_RELAY:
			{
				BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(MessageID)*2);
				PacketPriority priority;
				PacketReliability reliability;
				char orderingChannel;
				unsigned char cIn;
				bsIn.Read(cIn);
				priority = (PacketPriority) cIn;
				bsIn.Read(cIn);
				reliability = (PacketReliability) cIn;
				bsIn.Read(orderingChannel);
				RakString key;
				bsIn.ReadCompressed(key);
				BitStream bsData;
				bsIn.Read(&bsData);
				StrAndGuidAndRoom **strAndGuid = strToGuidHash.Peek(key);
				StrAndGuidAndRoom **strAndGuidSender = guidToStrHash.Peek(packet->guid);
				if (strAndGuid && strAndGuidSender)
				{
					BitStream bsOut;
					bsOut.WriteCasted<MessageID>(ID_RELAY_PLUGIN);
					bsOut.WriteCasted<MessageID>(FROM_RELAY);
					bsOut.Write( (*strAndGuidSender)->str );
					bsOut.Write(bsData);
					SendUnified(&bsOut, priority, reliability, orderingChannel, (*strAndGuid)->guid, false);
				}

				return RR_STOP_PROCESSING_AND_DEALLOCATE;
			}

		case ADD_CLIENT:
			{
				BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(MessageID)*2);
				RakString key;
				bsIn.ReadCompressed(key);
				if (acceptAddParticipantRequests)
				{
					BitStream bsOut;
					bsOut.WriteCasted<MessageID>(ID_RELAY_PLUGIN);
					bsOut.WriteCasted<MessageID>(ADD_CLIENT_RESPONSE);
					if (AddParticipantOnServer(key, packet->guid))
						bsOut.Write(true);
					else
						bsOut.Write(false);
					bsOut.WriteCompressed(key);
					SendUnified(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				}

				return RR_STOP_PROCESSING_AND_DEALLOCATE;
			}
		case REMOVE_CLIENT:
			{
				RemoveParticipantOnServer(packet->guid);
			}
			return RR_STOP_PROCESSING_AND_DEALLOCATE;
		}

	}
	
	return RR_CONTINUE_PROCESSING;
}

void RelayPlugin::OnClosedConnection(const SystemAddress &systemAddress, RakNetGUID rakNetGUID, PI2_LostConnectionReason lostConnectionReason )
{
	(void) lostConnectionReason;
	(void) systemAddress;

	RemoveParticipantOnServer(rakNetGUID);
}
void RelayPlugin::JoinRoom(RakNetGUID userGuid, RakString roomName)
{
	StrAndGuidAndRoom **strAndGuidSender = guidToStrHash.Peek(userGuid);
	if (strAndGuidSender)
	{
		if (roomName.IsEmpty())
			return;

		if ((*strAndGuidSender)->currentRoom==roomName)
			return;

		if ((*strAndGuidSender)->currentRoom.IsEmpty()==false)
			LeaveRoom(strAndGuidSender);

		RakString userName = (*strAndGuidSender)->str;

		for (unsigned int i=0; i < chatRooms.Size(); i++)
		{
			if (chatRooms[i]->roomName==roomName)
			{
				// Join existing room
				JoinRoom(chatRooms[i],strAndGuidSender);
				return;
			}
		}

		// Create new room
		RP_ChatRoom *room = RakNet::OP_NEW<RP_ChatRoom>(_FILE_AND_LINE_);
		room->roomName=roomName;
		chatRooms.Push(room, _FILE_AND_LINE_);
		JoinRoom(room,strAndGuidSender);
	}
}
void RelayPlugin::JoinRoom(RP_ChatRoom* room, StrAndGuidAndRoom **strAndGuidSender)
{
	if (strAndGuidSender==0)
		return;

	NotifyUsersInRoom(room, USER_ENTERED_ROOM, (*strAndGuidSender)->str);
	StrAndGuid sag;
	sag.guid=(*strAndGuidSender)->guid;
	sag.str=(*strAndGuidSender)->str;

	room->usersInRoom.Push(sag, _FILE_AND_LINE_);
	(*strAndGuidSender)->currentRoom=room->roomName;
}
void RelayPlugin::LeaveRoom(StrAndGuidAndRoom **strAndGuidSender)
{
	if (strAndGuidSender==0)
		return;

	RakString userName = (*strAndGuidSender)->str;
	for (unsigned int i=0; i < chatRooms.Size(); i++)
	{
		if (chatRooms[i]->roomName==(*strAndGuidSender)->currentRoom)
		{
			(*strAndGuidSender)->currentRoom.Clear();

			RP_ChatRoom *room = chatRooms[i];
			for (unsigned int j=0; j < room->usersInRoom.Size(); j++)
			{
				if (room->usersInRoom[j].guid==(*strAndGuidSender)->guid)
				{
					room->usersInRoom.RemoveAtIndexFast(j);

					if (room->usersInRoom.Size()==0)
					{
						RakNet::OP_DELETE(room, _FILE_AND_LINE_);
						chatRooms.RemoveAtIndexFast(i);
						return;
					}
				}
			}

			NotifyUsersInRoom(room, USER_LEFT_ROOM, userName);

			return;

		}
	}
}
void RelayPlugin::NotifyUsersInRoom(RP_ChatRoom *room, int msg, RakString str)
{
	for (unsigned int i=0; i < room->usersInRoom.Size(); i++)
	{
		BitStream bsOut;
		bsOut.WriteCasted<MessageID>(ID_RELAY_PLUGIN);
		bsOut.WriteCasted<MessageID>(msg);
		bsOut.WriteCompressed(msg);

		SendUnified(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, room->usersInRoom[i].guid, false);
	}
}

void RelayPlugin::SendMessageToRoom(StrAndGuidAndRoom **strAndGuidSender, RakString message)
{
	for (unsigned int i=0; i < chatRooms.Size(); i++)
	{
		if (chatRooms[i]->roomName==(*strAndGuidSender)->currentRoom)
		{
			RP_ChatRoom *room = chatRooms[i];
			NotifyUsersInRoom(room, ROOM_CHAT_MSG, message);
			break;
		}
	}
}
void RelayPlugin::SendChatRoomsList(RakNetGUID target)
{
	BitStream bsOut;
	bsOut.WriteCasted<MessageID>(ID_RELAY_PLUGIN);
	bsOut.WriteCasted<MessageID>(CHAT_ROOM_LIST);
	bsOut.WriteCasted<uint16_t>(chatRooms.Size());
	for (unsigned int i=0; i < chatRooms.Size(); i++)
	{
		bsOut.WriteCompressed(chatRooms[i]->roomName);
		bsOut.WriteCasted<uint16_t>(chatRooms[i]->usersInRoom.Size());
	}
	SendUnified(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, target, false);
}

#endif // _RAKNET_SUPPORT_*
*/
