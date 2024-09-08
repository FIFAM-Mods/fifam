#pragma once
#include "FifamReadWrite.h"

class UpdateKitConfig {
public:
	UpdateKitConfig() {
		Set<String> kitsToUpdate;
		Path updatedKitsPath = R"(C:\Users\user\Desktop\FM 23 update 1\_KITS\data\kits)";
		for (const auto &i : directory_iterator(updatedKitsPath)) {
			auto const &p = i.path();
			if (p.extension().string() == ".tga")
				kitsToUpdate.insert(p.stem().c_str());
		}
		struct KitEntry {
			String countryName, leagueName, teamName, teamid;
			Int teamkittypetechid = 0;
			UChar collar = 0, nameplacement = 0, frontnumber = 0, jerseynumbersize = 0, jerseynumberoffset = 0, canusecompbadges = 0, canusesponsorlogo = 0;
			Char jerseynumbercolor = 0, jerseynamecolor = 0;
		};
		Vector<KitEntry> kitEntries;
		Vector<KitEntry> kitEntriesUpdated;
		Vector<KitEntry> kitEntriesAdded;
		{
			FifamReader reader("D:\\Games\\FIFA Manager 22\\plugins\\ucp\\kits.csv", 14, FifamVersion());
			reader.SkipLine();
			while (!reader.IsEof()) {
				if (!reader.EmptyLine()) {
					KitEntry e;
					reader.ReadLine(e.countryName, e.leagueName, e.teamName, e.teamid, e.teamkittypetechid, e.collar, e.nameplacement,
						e.frontnumber, e.jerseynumbercolor, e.jerseynamecolor, e.jerseynumbersize, e.jerseynumberoffset, e.canusecompbadges,
						e.canusesponsorlogo);
					kitEntries.push_back(e);
				}
				else
					reader.SkipLine();
			}
		}
		{
			FifamReader reader("D:\\Games\\FIFA Manager 22\\plugins\\ucp\\kits_update.csv", 14, FifamVersion());
			FifamWriter updatesWriter("D:\\Games\\FIFA Manager 22\\plugins\\ucp\\kits_new_updates.csv", 14, FifamVersion());
			reader.SkipLine();
			while (!reader.IsEof()) {
				if (!reader.EmptyLine()) {
					KitEntry e;
					reader.ReadLine(e.countryName, e.leagueName, e.teamName, e.teamid, e.teamkittypetechid, e.collar, e.nameplacement,
						e.frontnumber, e.jerseynumbercolor, e.jerseynamecolor, e.jerseynumbersize, e.jerseynumberoffset, e.canusecompbadges,
						e.canusesponsorlogo);
					String kitId = String(L"00") + e.teamid.substr(2) + L"_";
					if (e.teamkittypetechid == 0)
						kitId += L"h";
					else if (e.teamkittypetechid == 1)
						kitId += L"a";
					else if (e.teamkittypetechid == 2)
						kitId += L"g";
					else if (e.teamkittypetechid == 3)
						kitId += L"t";
					if (Utils::Contains(kitsToUpdate, kitId)) {
						bool updatedExistingEntry = false;
						for (auto &existingEntry : kitEntries) {
							if (existingEntry.teamid == e.teamid && existingEntry.teamkittypetechid == e.teamkittypetechid) {
								// update existing entry
								existingEntry.collar = e.collar;
								existingEntry.nameplacement = e.nameplacement;
								existingEntry.jerseynumbercolor = e.jerseynumbercolor;
								existingEntry.jerseynamecolor = e.jerseynamecolor;
								kitEntriesUpdated.push_back(existingEntry);
								updatedExistingEntry = true;
								break;
							}
						}
						if (!updatedExistingEntry) {
							kitEntries.push_back(e);
							kitEntriesAdded.push_back(e);
						}
					}
				}
				else
					reader.SkipLine();
			}
		}
		{
			FifamWriter writer("D:\\Games\\FIFA Manager 22\\plugins\\ucp\\kits_new.csv", 14, FifamVersion());
			writer.WriteLine(L"country,league,team,teamid,kittype,collar,nameplacement,frontnumber,jerseynumbercolor,jerseynamecolor,jerseynumbersize,jerseynumberoffset,canusecompbadges,canusesponsorlogo");
			for (auto &e : kitEntries) {
				writer.WriteLine(Quoted(e.countryName), Quoted(e.leagueName), Quoted(e.teamName), e.teamid, e.teamkittypetechid, e.collar, e.nameplacement,
					e.frontnumber, e.jerseynumbercolor, e.jerseynamecolor, e.jerseynumbersize, e.jerseynumberoffset, e.canusecompbadges,
					e.canusesponsorlogo);
			}
		}
		{
			FifamWriter writer("D:\\Games\\FIFA Manager 22\\plugins\\ucp\\kits_new_updates.csv", 14, FifamVersion());
			writer.WriteLine(L"country,league,team,teamid,kittype,collar,nameplacement,frontnumber,jerseynumbercolor,jerseynamecolor,jerseynumbersize,jerseynumberoffset,canusecompbadges,canusesponsorlogo");
			for (auto &e : kitEntriesUpdated) {
				writer.WriteLine(Quoted(e.countryName), Quoted(e.leagueName), Quoted(e.teamName), e.teamid, e.teamkittypetechid, e.collar, e.nameplacement,
					e.frontnumber, e.jerseynumbercolor, e.jerseynamecolor, e.jerseynumbersize, e.jerseynumberoffset, e.canusecompbadges,
					e.canusesponsorlogo);
			}
			for (auto &e : kitEntriesAdded) {
				writer.WriteLine(Quoted(e.countryName), Quoted(e.leagueName), Quoted(e.teamName), e.teamid, e.teamkittypetechid, e.collar, e.nameplacement,
					e.frontnumber, e.jerseynumbercolor, e.jerseynamecolor, e.jerseynumbersize, e.jerseynumberoffset, e.canusecompbadges,
					e.canusesponsorlogo);
			}
		}
	}
};
