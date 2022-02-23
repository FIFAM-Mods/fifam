#include "Fifa07Kit.h"
#include <sstream>
#include <string>

void Fifa07Kit::Parse(Fifa07DataFile::Line & line) {
    line >> teamid >> kittype >> wetkit >> wrinkle >> collargeometry >> nameplacement >> namecolor >> kitfont >> jerseytype >> jerseyfontsize >> numberplacementback >> jerseydoubledigitfontsize >> numberplacementfront >> numbercolor >> jerseycolor1 >> jerseycolor2 >> jerseycolor3 >> shorttype >> shortcolor1 >> shortcolor2 >> shortcolor3 >> shortnumberplacement >> shortnumberfontsize >> shortnumbercolor >> shortnumberx >> shortnumbery >> socktype >> sockcolor1 >> sockcolor2 >> sockcolor3 >> jerseynumbercolorid >> shortsnumbercolorid >> shortsnumberhotspotid >> kitclashavrhsv_h >> kitclashavrhsv_s >> kitclashavrhsv_v;
}

Fifa07Kit::Fifa07Kit(Fifa07DataFile::Line & line) {
    Parse(line);
}

unsigned int Fifa07Kit::GetId() {
    return teamid;
}
