//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CModelManager.cpp
// Project: Client.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "CModelManager.h"
#include "Scripting.h"
#include "CGame.h"

DWORD CModelManager::VehicleIdToModelHash(int iModelId)
{
	switch(iModelId)
	{
	case 0: return Scripting::MODEL_ADMIRAL;
	case 1: return Scripting::MODEL_AIRTUG;
	case 2: return Scripting::MODEL_AMBULANCE;
	case 3: return Scripting::MODEL_BANSHEE;
	case 4: return Scripting::MODEL_BENSON;
	case 5: return Scripting::MODEL_BIFF;
	case 6: return Scripting::MODEL_BLISTA;
	case 7: return Scripting::MODEL_BOBCAT;
	case 8: return Scripting::MODEL_BOXVILLE;
	case 9: return Scripting::MODEL_BUCCANEER;
	case 10: return Scripting::MODEL_BURRITO;
	case 11: return Scripting::MODEL_BURRITO2;
	case 12: return Scripting::MODEL_BUS;
	case 13: return Scripting::MODEL_CABBY;
	case 14: return Scripting::MODEL_CAVALCADE;
	case 15: return Scripting::MODEL_CHAVOS;
	case 16: return Scripting::MODEL_COGNOSCENTI;
	case 17: return Scripting::MODEL_COMET;
	case 18: return Scripting::MODEL_COQUETTE;
	case 19: return Scripting::MODEL_DF8;
	case 20: return Scripting::MODEL_DILETTANTE;
	case 21: return Scripting::MODEL_DUKES;
	case 22: return Scripting::MODEL_E109;
	case 23: return Scripting::MODEL_EMPEROR;
	case 24: return Scripting::MODEL_EMPEROR2;
	case 25: return Scripting::MODEL_ESPERANTO;
	case 26: return Scripting::MODEL_FACTION;
	case 27: return Scripting::MODEL_FBI;
	case 28: return Scripting::MODEL_FELTZER;
	case 29: return Scripting::MODEL_FEROCI;
	case 30: return Scripting::MODEL_FEROCI2;
	case 31: return Scripting::MODEL_FIRETRUK;
	case 32: return Scripting::MODEL_FLATBED;
	case 33: return Scripting::MODEL_FORTUNE;
	case 34: return Scripting::MODEL_FORKLIFT;
	case 35: return Scripting::MODEL_FUTO;
	case 36: return Scripting::MODEL_FXT;
	case 37: return Scripting::MODEL_HABANERO;
	case 38: return Scripting::MODEL_HAKUMAI;
	case 39: return Scripting::MODEL_HUNTLEY;
	case 40: return Scripting::MODEL_INFERNUS;
	case 41: return Scripting::MODEL_INGOT;
	case 42: return Scripting::MODEL_INTRUDER;
	case 43: return Scripting::MODEL_LANDSTALKER;
	case 44: return Scripting::MODEL_LOKUS;
	case 45: return Scripting::MODEL_MANANA;
	case 46: return Scripting::MODEL_MARBELLA;
	case 47: return Scripting::MODEL_MERIT;
	case 48: return Scripting::MODEL_MINIVAN;
	case 49: return Scripting::MODEL_MOONBEAM;
	case 50: return Scripting::MODEL_MRTASTY;
	case 51: return Scripting::MODEL_MULE;
	case 52: return Scripting::MODEL_NOOSE;
	case 53: return Scripting::MODEL_NSTOCKADE;
	case 54: return Scripting::MODEL_ORACLE;
	case 55: return Scripting::MODEL_PACKER;
	case 56: return Scripting::MODEL_PATRIOT;
	case 57: return Scripting::MODEL_PERENNIAL;
	case 58: return Scripting::MODEL_PERENNIAL2;
	case 59: return Scripting::MODEL_PEYOTE;
	case 60: return Scripting::MODEL_PHANTOM;
	case 61: return Scripting::MODEL_PINNACLE;
	case 62: return Scripting::MODEL_PMP600;
	case 63: return Scripting::MODEL_POLICE;
	case 64: return Scripting::MODEL_POLICE2;
	case 65: return Scripting::MODEL_POLPATRIOT;
	case 66: return Scripting::MODEL_PONY;
	case 67: return Scripting::MODEL_PREMIER;
	case 68: return Scripting::MODEL_PRES;
	case 69: return Scripting::MODEL_PRIMO;
	case 70: return Scripting::MODEL_PSTOCKADE;
	case 71: return Scripting::MODEL_RANCHER;
	case 72: return Scripting::MODEL_REBLA;
	case 73: return Scripting::MODEL_RIPLEY;
	case 74: return Scripting::MODEL_ROMERO;
	case 75: return Scripting::MODEL_ROM;
	case 76: return Scripting::MODEL_RUINER;
	case 77: return Scripting::MODEL_SABRE;
	case 78: return Scripting::MODEL_SABRE2;
	case 79: return Scripting::MODEL_SABREGT;
	case 80: return Scripting::MODEL_SCHAFTER;
	case 81: return Scripting::MODEL_SENTINEL;
	case 82: return Scripting::MODEL_SOLAIR;
	case 83: return Scripting::MODEL_SPEEDO;
	case 84: return Scripting::MODEL_STALION;
	case 85: return Scripting::MODEL_STEED;
	case 86: return Scripting::MODEL_STOCKADE;
	case 87: return Scripting::MODEL_STRATUM;
	case 88: return Scripting::MODEL_STRETCH;
	case 89: return Scripting::MODEL_SULTAN;
	case 90: return Scripting::MODEL_SULTANRS;
	case 91: return Scripting::MODEL_SUPERGT;
	case 92: return Scripting::MODEL_TAXI;
	case 93: return Scripting::MODEL_TAXI2;
	case 94: return Scripting::MODEL_TRASH;
	case 95: return Scripting::MODEL_TURISMO;
	case 96: return Scripting::MODEL_URANUS;
	case 97: return Scripting::MODEL_VIGERO;
	case 98: return Scripting::MODEL_VIGERO2;
	case 99: return Scripting::MODEL_VINCENT;
	case 100: return Scripting::MODEL_VIRGO;
	case 101: return Scripting::MODEL_VOODOO;
	case 102: return Scripting::MODEL_WASHINGTON;
	case 103: return Scripting::MODEL_WILLARD;
	case 104: return Scripting::MODEL_YANKEE;
	case 105: return Scripting::MODEL_BOBBER;
	case 106: return Scripting::MODEL_FAGGIO;
	case 107: return Scripting::MODEL_HELLFURY;
	case 108: return Scripting::MODEL_NRG900;
	case 109: return Scripting::MODEL_PCJ;
	case 110: return Scripting::MODEL_SANCHEZ;
	case 111: return Scripting::MODEL_ZOMBIEB;
	case 112: return Scripting::MODEL_ANNIHILATOR;
	case 113: return Scripting::MODEL_MAVERICK;
	case 114: return Scripting::MODEL_POLMAV;
	case 115: return Scripting::MODEL_TOURMAV;
	case 116: return Scripting::MODEL_DINGHY;
	case 117: return Scripting::MODEL_JETMAX;
	case 118: return Scripting::MODEL_MARQUIS;
	case 119: return Scripting::MODEL_PREDATOR;
	case 120: return Scripting::MODEL_REEFER;
	case 121: return Scripting::MODEL_SQUALO;
	case 122: return Scripting::MODEL_TUGA;
	case 123: return Scripting::MODEL_TROPIC;
	case 124: return Scripting::MODEL_CABLECAR;
	case 125: return Scripting::MODEL_SUBWAY_LO;
	case 126: return Scripting::MODEL_SUBWAY_HI;
	}

	return 0x00;
}

int CModelManager::ModelHashToVehicleId(DWORD dwModelHash)
{
	switch(dwModelHash)
	{
	case Scripting::MODEL_ADMIRAL: return 0;
	case Scripting::MODEL_AIRTUG: return 1;
	case Scripting::MODEL_AMBULANCE: return 2;
	case Scripting::MODEL_BANSHEE: return 3;
	case Scripting::MODEL_BENSON: return 4;
	case Scripting::MODEL_BIFF: return 5;
	case Scripting::MODEL_BLISTA: return 6;
	case Scripting::MODEL_BOBCAT: return 7;
	case Scripting::MODEL_BOXVILLE: return 8;
	case Scripting::MODEL_BUCCANEER: return 9;
	case Scripting::MODEL_BURRITO: return 10;
	case Scripting::MODEL_BURRITO2: return 11;
	case Scripting::MODEL_BUS: return 12;
	case Scripting::MODEL_CABBY: return 13;
	case Scripting::MODEL_CAVALCADE: return 14;
	case Scripting::MODEL_CHAVOS: return 15;
	case Scripting::MODEL_COGNOSCENTI: return 16;
	case Scripting::MODEL_COMET: return 17;
	case Scripting::MODEL_COQUETTE: return 18;
	case Scripting::MODEL_DF8: return 19;
	case Scripting::MODEL_DILETTANTE: return 20;
	case Scripting::MODEL_DUKES: return 21;
	case Scripting::MODEL_E109: return 22;
	case Scripting::MODEL_EMPEROR: return 23;
	case Scripting::MODEL_EMPEROR2: return 24;
	case Scripting::MODEL_ESPERANTO: return 25;
	case Scripting::MODEL_FACTION: return 26;
	case Scripting::MODEL_FBI: return 27;
	case Scripting::MODEL_FELTZER: return 28;
	case Scripting::MODEL_FEROCI: return 29;
	case Scripting::MODEL_FEROCI2: return 30;
	case Scripting::MODEL_FIRETRUK: return 31;
	case Scripting::MODEL_FLATBED: return 32;
	case Scripting::MODEL_FORTUNE: return 33;
	case Scripting::MODEL_FORKLIFT: return 34;
	case Scripting::MODEL_FUTO: return 35;
	case Scripting::MODEL_FXT: return 36;
	case Scripting::MODEL_HABANERO: return 37;
	case Scripting::MODEL_HAKUMAI: return 38;
	case Scripting::MODEL_HUNTLEY: return 39;
	case Scripting::MODEL_INFERNUS: return 40;
	case Scripting::MODEL_INGOT: return 41;
	case Scripting::MODEL_INTRUDER: return 42;
	case Scripting::MODEL_LANDSTALKER: return 43;
	case Scripting::MODEL_LOKUS: return 44;
	case Scripting::MODEL_MANANA: return 45;
	case Scripting::MODEL_MARBELLA: return 46;
	case Scripting::MODEL_MERIT: return 47;
	case Scripting::MODEL_MINIVAN: return 48;
	case Scripting::MODEL_MOONBEAM: return 49;
	case Scripting::MODEL_MRTASTY: return 50;
	case Scripting::MODEL_MULE: return 51;
	case Scripting::MODEL_NOOSE: return 52;
	case Scripting::MODEL_NSTOCKADE: return 53;
	case Scripting::MODEL_ORACLE: return 54;
	case Scripting::MODEL_PACKER: return 55;
	case Scripting::MODEL_PATRIOT: return 56;
	case Scripting::MODEL_PERENNIAL: return 57;
	case Scripting::MODEL_PERENNIAL2: return 58;
	case Scripting::MODEL_PEYOTE: return 59;
	case Scripting::MODEL_PHANTOM: return 60;
	case Scripting::MODEL_PINNACLE: return 61;
	case Scripting::MODEL_PMP600: return 62;
	case Scripting::MODEL_POLICE: return 63;
	case Scripting::MODEL_POLICE2: return 64;
	case Scripting::MODEL_POLPATRIOT: return 65;
	case Scripting::MODEL_PONY: return 66;
	case Scripting::MODEL_PREMIER: return 67;
	case Scripting::MODEL_PRES: return 68;
	case Scripting::MODEL_PRIMO: return 69;
	case Scripting::MODEL_PSTOCKADE: return 70;
	case Scripting::MODEL_RANCHER: return 71;
	case Scripting::MODEL_REBLA: return 72;
	case Scripting::MODEL_RIPLEY: return 73;
	case Scripting::MODEL_ROMERO: return 74;
	case Scripting::MODEL_ROM: return 75;
	case Scripting::MODEL_RUINER: return 76;
	case Scripting::MODEL_SABRE: return 77;
	case Scripting::MODEL_SABRE2: return 78;
	case Scripting::MODEL_SABREGT: return 79;
	case Scripting::MODEL_SCHAFTER: return 80;
	case Scripting::MODEL_SENTINEL: return 81;
	case Scripting::MODEL_SOLAIR: return 82;
	case Scripting::MODEL_SPEEDO: return 83;
	case Scripting::MODEL_STALION: return 84;
	case Scripting::MODEL_STEED: return 85;
	case Scripting::MODEL_STOCKADE: return 86;
	case Scripting::MODEL_STRATUM: return 87;
	case Scripting::MODEL_STRETCH: return 88;
	case Scripting::MODEL_SULTAN: return 89;
	case Scripting::MODEL_SULTANRS: return 90;
	case Scripting::MODEL_SUPERGT: return 91;
	case Scripting::MODEL_TAXI: return 92;
	case Scripting::MODEL_TAXI2: return 93;
	case Scripting::MODEL_TRASH: return 94;
	case Scripting::MODEL_TURISMO: return 95;
	case Scripting::MODEL_URANUS: return 96;
	case Scripting::MODEL_VIGERO: return 97;
	case Scripting::MODEL_VIGERO2: return 98;
	case Scripting::MODEL_VINCENT: return 99;
	case Scripting::MODEL_VIRGO: return 100;
	case Scripting::MODEL_VOODOO: return 101;
	case Scripting::MODEL_WASHINGTON: return 102;
	case Scripting::MODEL_WILLARD: return 103;
	case Scripting::MODEL_YANKEE: return 104;
	case Scripting::MODEL_BOBBER: return 105;
	case Scripting::MODEL_FAGGIO: return 106;
	case Scripting::MODEL_HELLFURY: return 107;
	case Scripting::MODEL_NRG900: return 108;
	case Scripting::MODEL_PCJ: return 109;
	case Scripting::MODEL_SANCHEZ: return 110;
	case Scripting::MODEL_ZOMBIEB: return 111;
	case Scripting::MODEL_ANNIHILATOR: return 112;
	case Scripting::MODEL_MAVERICK: return 113;
	case Scripting::MODEL_POLMAV: return 114;
	case Scripting::MODEL_TOURMAV: return 115;
	case Scripting::MODEL_DINGHY: return 116;
	case Scripting::MODEL_JETMAX: return 117;
	case Scripting::MODEL_MARQUIS: return 118;
	case Scripting::MODEL_PREDATOR: return 119;
	case Scripting::MODEL_REEFER: return 120;
	case Scripting::MODEL_SQUALO: return 121;
	case Scripting::MODEL_TUGA: return 122;
	case Scripting::MODEL_TROPIC: return 123;
	case Scripting::MODEL_CABLECAR: return 124;
	case Scripting::MODEL_SUBWAY_LO: return 125;
	case Scripting::MODEL_SUBWAY_HI: return 126;
	}

	return -1;
}
