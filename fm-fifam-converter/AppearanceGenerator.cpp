#include "AppearanceGenerator.h"
#include "FifamDatabase.h"
#include "FifamPlayer.h"
#include "FifaPlayer.h"
#include "FifamPlayerAppearance.h"
#include "FifaDatabase.h"
#include "Random.h"

Map<StringA, UInt> AppearanceGenerator::HeadIdFromEditorName = {
    {"Female - African 1",380},{"Female - African 2",381},{"Female - African 3",382},{"Female - African 4",383},{"Female - African 5",384},{"Female - African 6",385},{"Female - African 7",386},{"Female - African North 1",453},{"Female - African North 10",462},{"Female - African North 11",463},{"Female - African North 12",464},{"Female - African North 13",465},{"Female - African North 14",466},{"Female - African North 15",467},{"Female - African North 16",468},{"Female - African North 17",469},{"Female - African North 18",470},{"Female - African North 19",471},{"Female - African North 2",454},{"Female - African North 20",472},{"Female - African North 21",473},{"Female - African North 22",474},{"Female - African North 23",475},{"Female - African North 24",476},{"Female - African North 25",477},{"Female - African North 26",478},{"Female - African North 27",479},{"Female - African North 3",455},{"Female - African North 4",456},{"Female - African North 5",457},{"Female - African North 6",458},{"Female - African North 7",459},{"Female - African North 8",460},{"Female - African North 9",461},{"Female - African South 1",425},{"Female - African South 2",426},{"Female - African South Eur 1",480},{"Female - African South Eur 2",481},{"Female - African South Eur 3",482},{"Female - African South Eur 4",483},{"Female - African South Eur 5",484},{"Female - African South Eur 6",485},{"Female - African South Eur 7",486},{"Female - African South Eur 8",487},{"Female - Arabic 1",424},{"Female - Asian 1",350},{"Female - Asian 10",359},{"Female - Asian 11",360},{"Female - Asian 12",361},{"Female - Asian 13",362},{"Female - Asian 14",363},{"Female - Asian 15",364},{"Female - Asian 16",365},{"Female - Asian 17",366},{"Female - Asian 18",367},{"Female - Asian 19",368},{"Female - Asian 2",351},{"Female - Asian 20",369},{"Female - Asian 21",370},{"Female - Asian 22",371},{"Female - Asian 23",372},{"Female - Asian 24",373},{"Female - Asian 25",374},{"Female - Asian 26",375},{"Female - Asian 27",376},{"Female - Asian 28",377},{"Female - Asian 29",378},{"Female - Asian 3",352},{"Female - Asian 30",379},{"Female - Asian 4",353},{"Female - Asian 5",354},{"Female - Asian 6",355},{"Female - Asian 7",356},{"Female - Asian 8",357},{"Female - Asian 9",358},{"Female - Caucasian 1",338},{"Female - Caucasian 10",347},{"Female - Caucasian 11",348},{"Female - Caucasian 12",349},{"Female - Caucasian 2",339},{"Female - Caucasian 3",340},{"Female - Caucasian 4",341},{"Female - Caucasian 5",342},{"Female - Caucasian 6",343},{"Female - Caucasian 7",344},{"Female - Caucasian 8",345},{"Female - Caucasian 9",346},{"Female - East European 1",421},{"Female - East European 2",422},{"Female - East European 3",423},{"Female - Latin 1",387},{"Female - Latin 10",396},{"Female - Latin 11",397},{"Female - Latin 12",398},{"Female - Latin 13",399},{"Female - Latin 14",400},{"Female - Latin 15",401},{"Female - Latin 16",402},{"Female - Latin 17",403},{"Female - Latin 18",404},{"Female - Latin 19",405},{"Female - Latin 2",388},{"Female - Latin 20",406},{"Female - Latin 21",407},{"Female - Latin 22",408},{"Female - Latin 23",409},{"Female - Latin 24",410},{"Female - Latin 25",411},{"Female - Latin 26",412},{"Female - Latin 27",413},{"Female - Latin 28",414},{"Female - Latin 29",415},{"Female - Latin 3",389},{"Female - Latin 30",416},{"Female - Latin 31",417},{"Female - Latin 32",418},{"Female - Latin 33",419},{"Female - Latin 34",420},{"Female - Latin 4",390},{"Female - Latin 5",391},{"Female - Latin 6",392},{"Female - Latin 7",393},{"Female - Latin 8",394},{"Female - Latin 9",395},{"Female - North European 1",433},{"Female - North European 10",442},{"Female - North European 11",443},{"Female - North European 12",444},{"Female - North European 13",445},{"Female - North European 14",446},{"Female - North European 15",447},{"Female - North European 16",448},{"Female - North European 17",449},{"Female - North European 18",450},{"Female - North European 19",451},{"Female - North European 2",434},{"Female - North European 20",452},{"Female - North European 3",435},{"Female - North European 4",436},{"Female - North European 5",437},{"Female - North European 6",438},{"Female - North European 7",439},{"Female - North European 8",440},{"Female - North European 9",441},{"Female - South European 1",427},{"Female - South European 2",428},{"Female - South European 3",429},{"Female - South European 4",430},{"Female - South European 5",431},{"Female - South European 6",432},{"Male - African 1",132},{"Male - African 10",141},{"Male - African 11",142},{"Male - African 12",143},{"Male - African 13",144},{"Male - African 14",145},{"Male - African 15",146},{"Male - African 16",147},{"Male - African 17",148},{"Male - African 18",149},{"Male - African 19",150},{"Male - African 2",133},{"Male - African 20",151},{"Male - African 21",152},{"Male - African 22",153},{"Male - African 23",154},{"Male - African 24",155},{"Male - African 25",156},{"Male - African 26",157},{"Male - African 27",158},{"Male - African 28",159},{"Male - African 29",160},{"Male - African 3",134},{"Male - African 30",161},{"Male - African 31",162},{"Male - African 32",163},{"Male - African 33",164},{"Male - African 34",165},{"Male - African 35",166},{"Male - African 36",167},{"Male - African 37",168},{"Male - African 38",169},{"Male - African 39",170},{"Male - African 4",135},{"Male - African 40",171},{"Male - African 41",172},{"Male - African 42",173},{"Male - African 43",174},{"Male - African 44",175},{"Male - African 45",176},{"Male - African 46",177},{"Male - African 47",178},{"Male - African 48",179},{"Male - African 49",180},{"Male - African 5",136},{"Male - African 6",137},{"Male - African 7",138},{"Male - African 8",139},{"Male - African 9",140},{"Male - African North 1",330},{"Male - African North 2",331},{"Male - African North 3",332},{"Male - African North 4",333},{"Male - African South 1",285},{"Male - African South 2",286},{"Male - African South 3",287},{"Male - African South 4",288},{"Male - African South 5",289},{"Male - African South 6",290},{"Male - African South Eur 1",334},{"Male - African South Eur 2",335},{"Male - African South Eur 3",336},{"Male - African South Eur 4",337},{"Male - Arabic 1",126},{"Male - Arabic 10",266},{"Male - Arabic 11",267},{"Male - Arabic 12",268},{"Male - Arabic 13",269},{"Male - Arabic 14",270},{"Male - Arabic 15",271},{"Male - Arabic 16",272},{"Male - Arabic 17",273},{"Male - Arabic 18",274},{"Male - Arabic 19",275},{"Male - Arabic 2",127},{"Male - Arabic 20",276},{"Male - Arabic 21",277},{"Male - Arabic 22",278},{"Male - Arabic 23",279},{"Male - Arabic 24",280},{"Male - Arabic 25",281},{"Male - Arabic 26",282},{"Male - Arabic 27",283},{"Male - Arabic 28",284},{"Male - Arabic 3",128},{"Male - Arabic 4",129},{"Male - Arabic 5",130},{"Male - Arabic 6",131},{"Male - Arabic 7",263},{"Male - Arabic 8",264},{"Male - Arabic 9",265},{"Male - Asian 1",56},{"Male - Asian 10",65},{"Male - Asian 11",66},{"Male - Asian 12",67},{"Male - Asian 13",68},{"Male - Asian 14",69},{"Male - Asian 15",70},{"Male - Asian 16",71},{"Male - Asian 17",72},{"Male - Asian 18",73},{"Male - Asian 19",74},{"Male - Asian 2",57},{"Male - Asian 20",75},{"Male - Asian 21",76},{"Male - Asian 22",77},{"Male - Asian 23",78},{"Male - Asian 24",79},{"Male - Asian 25",80},{"Male - Asian 26",81},{"Male - Asian 27",82},{"Male - Asian 28",83},{"Male - Asian 29",84},{"Male - Asian 3",58},{"Male - Asian 30",85},{"Male - Asian 31",86},{"Male - Asian 32",87},{"Male - Asian 33",88},{"Male - Asian 34",89},{"Male - Asian 35",90},{"Male - Asian 36",91},{"Male - Asian 37",92},{"Male - Asian 38",93},{"Male - Asian 39",94},{"Male - Asian 4",59},{"Male - Asian 40",95},{"Male - Asian 41",96},{"Male - Asian 42",97},{"Male - Asian 43",98},{"Male - Asian 44",99},{"Male - Asian 45",100},{"Male - Asian 46",101},{"Male - Asian 47",102},{"Male - Asian 48",103},{"Male - Asian 49",104},{"Male - Asian 5",60},{"Male - Asian 50",105},{"Male - Asian 51",106},{"Male - Asian 52",107},{"Male - Asian 53",108},{"Male - Asian 54",109},{"Male - Asian 55",110},{"Male - Asian 56",111},{"Male - Asian 57",112},{"Male - Asian 58",113},{"Male - Asian 59",114},{"Male - Asian 6",61},{"Male - Asian 60",115},{"Male - Asian 61",116},{"Male - Asian 62",117},{"Male - Asian 63",118},{"Male - Asian 64",119},{"Male - Asian 65",120},{"Male - Asian 66",121},{"Male - Asian 67",122},{"Male - Asian 68",123},{"Male - Asian 69",124},{"Male - Asian 7",62},{"Male - Asian 70",125},{"Male - Asian 8",63},{"Male - Asian 9",64},{"Male - Caucasian 1",0},{"Male - Caucasian 10",9},{"Male - Caucasian 11",10},{"Male - Caucasian 12",11},{"Male - Caucasian 13",12},{"Male - Caucasian 14",13},{"Male - Caucasian 15",14},{"Male - Caucasian 16",15},{"Male - Caucasian 17",16},{"Male - Caucasian 18",17},{"Male - Caucasian 19",18},{"Male - Caucasian 2",1},{"Male - Caucasian 20",19},{"Male - Caucasian 21",20},{"Male - Caucasian 22",21},{"Male - Caucasian 23",22},{"Male - Caucasian 24",23},{"Male - Caucasian 25",24},{"Male - Caucasian 26",25},{"Male - Caucasian 27",26},{"Male - Caucasian 28",27},{"Male - Caucasian 29",28},{"Male - Caucasian 3",2},{"Male - Caucasian 30",29},{"Male - Caucasian 31",30},{"Male - Caucasian 32",31},{"Male - Caucasian 33",32},{"Male - Caucasian 34",33},{"Male - Caucasian 35",34},{"Male - Caucasian 36",35},{"Male - Caucasian 37",36},{"Male - Caucasian 38",37},{"Male - Caucasian 39",38},{"Male - Caucasian 4",3},{"Male - Caucasian 40",39},{"Male - Caucasian 41",40},{"Male - Caucasian 42",41},{"Male - Caucasian 43",42},{"Male - Caucasian 44",43},{"Male - Caucasian 45",44},{"Male - Caucasian 46",45},{"Male - Caucasian 47",46},{"Male - Caucasian 48",47},{"Male - Caucasian 49",48},{"Male - Caucasian 5",4},{"Male - Caucasian 50",49},{"Male - Caucasian 51",50},{"Male - Caucasian 52",51},{"Male - Caucasian 53",52},{"Male - Caucasian 54",53},{"Male - Caucasian 55",54},{"Male - Caucasian 56",55},{"Male - Caucasian 6",5},{"Male - Caucasian 7",6},{"Male - Caucasian 8",7},{"Male - Caucasian 9",8},{"Male - East European 1",233},{"Male - East European 10",242},{"Male - East European 11",243},{"Male - East European 12",244},{"Male - East European 13",245},{"Male - East European 14",246},{"Male - East European 15",247},{"Male - East European 16",248},{"Male - East European 17",249},{"Male - East European 18",250},{"Male - East European 19",251},{"Male - East European 2",234},{"Male - East European 20",252},{"Male - East European 21",253},{"Male - East European 22",254},{"Male - East European 23",255},{"Male - East European 24",256},{"Male - East European 25",257},{"Male - East European 26",258},{"Male - East European 27",259},{"Male - East European 28",260},{"Male - East European 29",261},{"Male - East European 3",235},{"Male - East European 30",262},{"Male - East European 4",236},{"Male - East European 5",237},{"Male - East European 6",238},{"Male - East European 7",239},{"Male - East European 8",240},{"Male - East European 9",241},{"Male - Latin 1",181},{"Male - Latin 10",190},{"Male - Latin 11",191},{"Male - Latin 12",192},{"Male - Latin 13",193},{"Male - Latin 14",194},{"Male - Latin 15",195},{"Male - Latin 16",196},{"Male - Latin 17",197},{"Male - Latin 18",198},{"Male - Latin 19",199},{"Male - Latin 2",182},{"Male - Latin 20",200},{"Male - Latin 21",201},{"Male - Latin 22",202},{"Male - Latin 23",203},{"Male - Latin 24",204},{"Male - Latin 25",205},{"Male - Latin 26",206},{"Male - Latin 27",207},{"Male - Latin 28",208},{"Male - Latin 29",209},{"Male - Latin 3",183},{"Male - Latin 30",210},{"Male - Latin 31",211},{"Male - Latin 32",212},{"Male - Latin 33",213},{"Male - Latin 34",214},{"Male - Latin 35",215},{"Male - Latin 36",216},{"Male - Latin 37",217},{"Male - Latin 38",218},{"Male - Latin 39",219},{"Male - Latin 4",184},{"Male - Latin 40",220},{"Male - Latin 41",221},{"Male - Latin 42",222},{"Male - Latin 43",223},{"Male - Latin 44",224},{"Male - Latin 45",225},{"Male - Latin 46",226},{"Male - Latin 47",227},{"Male - Latin 48",228},{"Male - Latin 49",229},{"Male - Latin 5",185},{"Male - Latin 50",230},{"Male - Latin 51",231},{"Male - Latin 52",232},{"Male - Latin 6",186},{"Male - Latin 7",187},{"Male - Latin 8",188},{"Male - Latin 9",189},{"Male - North European 1",309},{"Male - North European 10",318},{"Male - North European 11",319},{"Male - North European 12",320},{"Male - North European 13",321},{"Male - North European 14",322},{"Male - North European 15",323},{"Male - North European 16",324},{"Male - North European 17",325},{"Male - North European 18",326},{"Male - North European 19",327},{"Male - North European 2",310},{"Male - North European 20",328},{"Male - North European 21",329},{"Male - North European 3",311},{"Male - North European 4",312},{"Male - North European 5",313},{"Male - North European 6",314},{"Male - North European 7",315},{"Male - North European 8",316},{"Male - North European 9",317},{"Male - South European 1",291},{"Male - South European 10",300},{"Male - South European 11",301},{"Male - South European 12",302},{"Male - South European 13",303},{"Male - South European 14",304},{"Male - South European 15",305},{"Male - South European 16",306},{"Male - South European 17",307},{"Male - South European 18",308},{"Male - South European 2",292},{"Male - South European 3",293},{"Male - South European 4",294},{"Male - South European 5",295},{"Male - South European 6",296},{"Male - South European 7",297},{"Male - South European 8",298},{"Male - South European 9",299}
};

Map<UInt, UInt> AppearanceGenerator::HairIdFromEditorId = {
    {0,0},{1,1},{2,2},{3,3},{4,4},{5,5},{6,6},{7,7},{8,8},{9,9},{10,10},{11,11},{12,12},{13,13},{14,14},{15,15},{16,16},{17,17},{18,18},{19,19},{20,20},{21,21},{22,22},{23,23},{24,24},{25,25},{26,26},{27,27},{28,28},{29,29},{30,30},{31,31},{32,32},{33,33},{34,34},{35,35},{36,36},{37,37},{38,38},{39,39},{40,40},{41,41},{42,42},{43,43},{44,44},{45,45},{46,46},{47,47},{48,48},{49,49},{51,50},{52,51},{53,52},{54,53},{55,54},{56,55},{57,56},{58,57},{59,58},{60,59},{61,60},{62,61},{63,62},{64,63},{65,64},{66,65},{67,66},{68,67},{69,68},{70,69},{71,70},{72,71},{73,72},{74,73},{75,74},{76,75},{77,76},{78,77},{79,78},{80,79},{81,80},{82,81},{83,82},{84,83},{85,84},{86,85},{87,86},{88,87},{89,88},{90,89},{91,90},{92,91},{93,92},{94,93},{95,94},{96,95},{97,96},{98,97},{99,98},{100,99},{101,100},{102,101},{103,102},{104,103},{105,104},{106,105},{107,106},{108,107},{109,108},{110,109},{111,110},{112,111},{113,112},{114,113},{115,114},{116,115},{117,116},{118,117},{119,118},{120,119},{121,120},{122,121},{123,122},{124,123},{125,124},{126,125},{127,126},{128,127},{129,128},{130,129},{131,130},{132,131},{133,132},{134,133},{135,134},{136,135},{137,136},{138,137},{139,138},{140,139},{141,140},{142,141},{143,142},{144,143},{145,144},{146,145},{147,146},{148,147},{149,148},{150,149},{151,150},{152,151},{153,152},{154,153},{155,154},{156,155},{157,156},{158,157},{159,158},{160,159},{161,160},{162,161},{163,162},{164,163},{165,164},{166,165},{167,166},{168,167},{169,168},{170,169},{171,170},{172,171},{173,172},{174,173},{175,174},{176,175},{177,176},{178,177},{179,178},{180,179},{181,180},{182,181},{183,182},{184,183},{185,184},{186,185},{187,186},{188,187},{189,188},{190,189},{191,190},{192,191},{193,192},{194,193},{195,194},{196,195},{197,196},{198,197},{199,198},{200,199},{201,200},{202,201},{203,202},{204,203},{205,204},{206,205},{207,206},{208,207},{209,208},{210,209},{211,210},{212,211},{218,212},{219,213},{220,214},{221,215},{222,216},{223,217},{224,218},{225,219},{226,220},{227,221},{228,222},{229,223},{230,224},{231,225},{232,226},{233,227},{234,228},{235,229},{236,230},{237,231},{238,232},{239,233},{240,234},{241,235},{242,236},{243,237},{244,238},{245,239},{246,240},{250,241},{251,242},{253,243},{255,244},{256,245},{257,246},{258,247},{260,248},{261,249},{262,250},{263,251},{264,252},{265,253},{266,254},{267,255},{268,256},{269,257},{270,258},{271,259},{272,260},{273,261},{274,262},{275,263},{276,264},{277,265},{278,266},{279,267},{280,268},{281,269},{282,270},{283,271},{284,272},{285,273},{286,274},{288,275},{289,276},{290,277},{291,278},{292,279},{293,280},{294,281},{295,282},{296,283},{297,284},{298,285},{299,286},{374,287},{375,288},{376,289},{381,290},{385,291},{386,292},{389,293},{390,294},{392,295},{394,296},{398,297},{399,298},{418,299},{419,300},{421,301},{431,302},{432,303},{433,304},{434,305},{435,306},{436,307},{437,308},{438,309},{444,310},{445,311},{446,312},{447,313},{448,314},{450,315},{451,316},{452,317},{460,318},{461,319},{462,320},{463,321},{492,322},{494,323},{500,324},{501,325},{502,326},{503,327},{504,328},{505,329},{506,330},{507,331},{508,332},{509,333},{510,334},{511,335},{512,336},{513,337},{514,338},{515,339},{516,340},{517,341},{518,342},{519,343},{520,344},{521,345},{522,346},{523,347},{524,348},{525,349},{526,350},{527,351},{528,352},{529,353},{530,354},{531,355},{532,356},{533,357},{534,358},{535,359},{536,360},{537,361},{538,362},{539,363},{540,364},{541,365},{542,366},{543,367},{544,368},{545,369},{546,370},{547,371},{548,372},{549,373},{550,374},{551,375},{552,376},{553,377},{554,378},{555,379},{556,380},{557,381},{558,382},{559,383},{560,384},{561,385},{562,386},{563,387},{564,388},{565,389},{566,390},{567,391},{568,392},{569,393},{570,394},{571,395},{572,396},{573,397},{574,398},{575,399},{576,400},{577,401},{578,402},{579,403},{580,404},{581,405},{582,406},{583,407},{584,408},{585,409},{586,410},{587,411},{588,412},{589,413},{590,414},{591,415},{592,416},{593,417},{594,418},{595,419},{596,420},{597,421},{598,422},{599,423},{600,424},{601,425},{602,426},{603,427},{604,428},{605,429},{606,430},{607,431},{608,432},{609,433},{610,434},{611,435},{612,436},{613,437},{614,438},{615,439},{616,440},{617,441},{618,442},{619,443},{620,444},{621,445},{622,446},{623,447},{624,448},{625,449},{626,450},{627,451},{628,452},{629,453},{630,454},{631,455},{632,456},{633,457},{634,458},{635,459},{636,460},{637,461},{638,462},{639,463},{640,464},{641,465},{642,466},{643,467},{644,468},{645,469},{646,470},{647,471},{648,472},{649,473},{650,474},{651,475},{652,476},{653,477},{654,478},{655,479},{656,480},{657,481},{659,482},{660,483},{661,484},{662,485},{663,486},{664,487},{665,488},{666,489},{667,490},{668,491},{669,492},{670,493},{671,494},{672,495},{673,496},{674,497},{675,498},{676,499},{677,500},{678,501},{679,502},{680,503},{681,504},{682,505},{683,506},{684,507},{685,508},{686,509},{687,510},{688,511},{689,512},{690,513},{691,514},{692,515},{693,516},{694,517},{695,518},{696,519},{697,520},{698,521},{720,522},{721,523},{722,524},{723,525},{724,526},{725,527},{726,528},{727,529},{728,530},{729,531},{730,532},{731,533},{732,534},{733,535},{734,536},{735,537},{736,538},{737,539},{738,540},{739,541},{740,542},{741,543},{742,544},{743,545},{744,546},{745,547},{746,548},{747,549},{748,550},{749,551},{810,552},{811,553},{815,554},{816,555},{828,556},{831,557},{832,558},{845,559},{881,560},{890,561},{891,562},{892,563},{900,564},{901,565},{902,566},{903,567},{904,568},{905,569},{906,570},{907,571},{908,572},{909,573},{910,574},{911,575},{912,576},{913,577},{914,578},{915,579},{916,580},{917,581},{918,582},{919,583},{920,584},{921,585},{922,586},{923,587},{924,588},{925,589},{926,590},{927,591},{928,592},{930,593},{931,594},{932,595},{1059,596},{1060,597},{1061,598},{1062,599},{1063,600},{1064,601},{1065,602},{1066,603},{1067,604},{1068,605},{1069,606},{1070,607},{1071,608},{1072,609},{1073,610},{1074,611},{1075,612},{1076,613},{1077,614},{1078,615},{1079,616},{1080,617},{1081,618},{1082,619},{1083,620},{1084,621},{1085,622},{1086,623},{1087,624},{1088,625},{1089,626},{1090,627},{1091,628},{1092,629},{1093,630},{1094,631},{1095,632},{1096,633},{1099,634},{1106,635},{1157,636},{1281,637},{1282,638},{1283,639},{1284,640},{1285,641},{1316,642},{1317,643},{1337,644},{1338,645},{1341,646},{1368,647},{1370,648},{1373,649},{1374,650},{1377,651},{1378,652},{1384,653},{1385,654},{1386,655},{2000,656},{2001,657},{2002,658},{2003,659},{2004,660},{2005,661},{2006,662},{2007,663},{2008,664},{2009,665},{2010,666},{2011,667},{2012,668},{2013,669},{2014,670},{2015,671},{2016,672},{2017,673},{2018,674},{2019,675},{2020,676},{2021,677},{2022,678},{2023,679},{2024,680},{2025,681},{2026,682},{2027,683},{2028,684},{2029,685},{2030,686},{2031,687},{2032,688},{2033,689},{2034,690},{2035,691},{2036,692},{2037,693},{2038,694},{2039,695},{2040,696},{2041,697},{2042,698},{2043,699},{2044,700},{2045,701},{2046,702},{2047,703},{2048,704},{2049,705},{2050,706},{2051,707},{2052,708},{2053,709},{2054,710},{2055,711},{2056,712},{2057,713},{2058,714},{2059,715},{2060,716},{2061,717},{2062,718},{2063,719},{2064,720},{2065,721},{2066,722},{2067,723},{2068,724},{3000,725},{3001,726},{3002,727},{3003,728},{3004,729},{3005,730},{3006,731},{3007,732},{3008,733},{3009,734},{3010,735},{3011,736},{3012,737},{3013,738},{3014,739},{3015,740},{3016,741},{3017,742},{3018,743},{3019,744},{3020,745},{3021,746},{3022,747},{3025,748},{3026,749},{3027,750},{3028,751},{3029,752},{3030,753},{3031,754},{3032,755},{3033,756},{3034,757},{3035,758},{3036,759},{3037,760},{3038,761},{3039,762},{3040,763},{3041,764},{3042,765},{3043,766},{3044,767},{3045,768},{3046,769},{3047,770},{3048,771},{3049,772},{3050,773},{3051,774},{3052,775},{3053,776},{3055,777},{3056,778},{3057,779},{3058,780},{3059,781},{3060,782},{3061,783},{3062,784},{3063,785},{3064,786},{3065,787},{3066,788},{3067,789},{3068,790},{3069,791},{3070,792},{3071,793},{3072,794},{3073,795},{3074,796},{3075,797},{3076,798},{3077,799},{3078,800},{3079,801},{3080,802},{3081,803},{3082,804},{3083,805},{3084,806},{3085,807},{3086,808},{3087,809},{3088,810},{3089,811},{3090,812},{3091,813},{3092,814},{3093,815},{3094,816},{3095,817},{3096,818},{3097,819},{3098,820},{3100,821},{3101,822},{3102,823},{3103,824},{3104,825},{3105,826},{3106,827},{3107,828},{3108,829},{3109,830},{3110,831},{3111,832},{3112,833},{3113,834},{3114,835},{3115,836},{3116,837},{3117,838},{3118,839},{3119,840},{3120,841},{3121,842},{3122,843},{3123,844},{3124,845},{3125,846},{3126,847},{3127,848},{3128,849},{3129,850},{3130,851},{3131,852},{3132,853},{3133,854},{3134,855},{3135,856},{3136,857},{3137,858},{3138,859},{3139,860},{3140,861},{3141,862},{3142,863},{3143,864},{3144,865},{3145,866},{3146,867},{3147,868},{3148,869},{3149,870},{3150,871},{3151,872},{3152,873},{3153,874},{3154,875},{3155,876},{3156,877},{3157,878},{3158,879},{3159,880},{3160,881},{3161,882},{3162,883},{3163,884},{3164,885},{3165,886},{3166,887},{3167,888},{3168,889},{3169,890},{3170,891},{3171,892},{3172,893},{3173,894},{3174,895},{3175,896},{3176,897},{3177,898},{3178,899},{3179,900},{3180,901},{3181,902},{3182,903},{3183,904},{3184,905},{3185,906},{3186,907},{3187,908},{3188,909},{3189,910},{3190,911},{3191,912},{3192,913},{3193,914},{3194,915},{3195,916},{3196,917},{3197,918},{3198,919},{3199,920},{3200,921},{3201,922},{3202,923},{3203,924},{3204,925},{3205,926},{3206,927},{3207,928},{3208,929},{3209,930},{3210,931},{3211,932},{3212,933},{3213,934},{3214,935},{3215,936},{3216,937},{3217,938},{3218,939},{3219,940},{3220,941},{3221,942},{3232,943},{3233,944},{3234,945},{3237,946},{3238,947},{3239,948},{3240,949},{3241,950},{3242,951},{3244,952},{3245,953},{3246,954},{3247,955},{3248,956},{3249,957},{3250,958},{3251,959},{3252,960},{3253,961},{3254,962},{3255,963},{3256,964},{3257,965},{3258,966},{3259,967},{3260,968},{3261,969},{3262,970},{3263,971},{3264,972},{3265,973},{3266,974},{3267,975},{3268,976},{3269,977},{3270,978},{3271,979},{3272,980},{3273,981},{3274,982},{3275,983},{3276,984},{3277,985},{3278,986},{3279,987},{3280,988},{3281,989},{3282,990},{3283,991},{3284,992},{3285,993},{3298,994},{3300,995},{4000,996},{4001,997},{4002,998},{4003,999},{4004,1000},{4005,1001},{4006,1002},{4007,1003},{4008,1004},{4009,1005},{4010,1006},{4011,1007},{4012,1008},{4013,1009},{4014,1010},{4015,1011},{4016,1012},{4017,1013},{4018,1014},{4019,1015},{4020,1016},{4021,1017},{4022,1018},{4023,1019},{4024,1020},{4025,1021},{4026,1022},{4027,1023},{4028,1024},{4029,1025},{4030,1026},{5000,1027},{5001,1028},{5002,1029},{5003,1030},{5004,1031},{5005,1032},{5006,1033},{5007,1034},{5008,1035},{5009,1036},{5010,1037},{5011,1038},{5012,1039},{5013,1040},{5014,1041},{5015,1042},{5016,1043},{5017,1044},{6000,1045},{6001,1046},{6002,1047},{6003,1048},{6004,1049},{6005,1050},{6006,1051},{6007,1052},{6008,1053},{6009,1054},{6010,1055},{6011,1056},{6012,1057},{6013,1058},{6014,1059},{6015,1060},{6016,1061},{6017,1062},{6018,1063},{6019,1064},{6020,1065},{6021,1066},{6022,1067},{6023,1068},{6024,1069},{6025,1070},{6026,1071},{6033,1072},{6035,1073},{6036,1074},{6037,1075},{6038,1076},{6039,1077},{6040,1078},{6041,1079},{6042,1080},{7007,1081},{7010,1082},{7013,1083},{7014,1084},{7018,1085},{7021,1086},{7025,1087},{7027,1088},{7034,1089},{7036,1090},{7037,1091},{7038,1092},{7045,1093},{7046,1094},{7047,1095},{7050,1096},{7051,1097},{7052,1098},{7054,1099},{7056,1100},{7057,1101},{7060,1102},{7070,1103},{7072,1104},{7080,1105},{7082,1106},{7083,1107},{7087,1108},{7095,1109},{7103,1110},{7117,1111},{7129,1112},{7136,1113},{7140,1114},{7145,1115},{7147,1116},{7155,1117},{7161,1118},{7163,1119},{7167,1120},{7181,1121},{7182,1122},{7183,1123},{7192,1124},{7195,1125},{7198,1126},{7251,1127},{7254,1128},{7257,1129},{7260,1130},{7262,1131},{7263,1132},{7264,1133},{7268,1134},{7270,1135},{7271,1136},{7273,1137},{7275,1138},{7277,1139},{7284,1140},{7286,1141},{7287,1142},{7288,1143},{7289,1144},{7292,1145},{7293,1146},{7294,1147},{7295,1148},{7296,1149},{7297,1150},{7298,1151},{7299,1152},{7300,1153},{7301,1154},{7302,1155},{7304,1156},{7306,1157},{7307,1158},{7310,1159},{7320,1160},{7322,1161},{7330,1162},{7332,1163},{7333,1164},{7337,1165},{7345,1166},{7353,1167},{7367,1168},{7379,1169},{7383,1170},{7386,1171},{7390,1172},{7395,1173},{7397,1174},{7405,1175},{7411,1176},{7413,1177},{7417,1178},{7431,1179},{7432,1180},{7433,1181},{7442,1182},{7445,1183},{7448,1184},{7538,1185},{7541,1186},{7544,1187},{7545,1188},{7549,1189},{7552,1190},{7556,1191},{7558,1192},{7565,1193},{7567,1194},{7568,1195},{7569,1196},{7576,1197},{7577,1198},{7578,1199},{7581,1200},{7582,1201},{7583,1202},{7585,1203},{7587,1204},{7588,1205},{7591,1206},{7601,1207},{7603,1208},{7611,1209},{7613,1210},{7614,1211},{7618,1212},{7626,1213},{7634,1214},{7648,1215},{7660,1216},{7664,1217},{7667,1218},{7671,1219},{7676,1220},{7678,1221},{7686,1222},{7692,1223},{7694,1224},{7698,1225},{7712,1226},{7713,1227},{7714,1228},{7723,1229},{7726,1230},{7729,1231}
};

Map<StringA, UInt> AppearanceGenerator::BeardIdFromEditorName = {
    {"Clean Shaved",0},{"Chin Stubble Light",1},{"Chin Strap",2},{"Goatee",3},{"Casual Beard",4},{"Partial Goatee",5},{"Stubble",6},{"Tuft",7},{"Full Beard",8},{"Light Goatee",9},{"Moustache",10},{"Light Chin Curtain",11},{"Full Goatee",12},{"Chin Curtain",13},{"Beard",14},{"Patchy Beard",15}
};
Map<StringA, UInt> AppearanceGenerator::EyeIdFromEditorName = {
    {"LightBlue",0},{"Brown",1},{"Gray-Green",2},{"Green",3},{"Green-Blue",4},{"Grey",5},{"Blue",6},{"Light Brown",7},{"Dark Brown",8},{"Yellow",9}
};

Vector<String> AppearanceGenerator::FaceNames = {
    L"Male - Caucasian 1",L"Male - Caucasian 2",L"Male - Caucasian 3",L"Male - Caucasian 4",L"Male - Caucasian 5",L"Male - Caucasian 6",L"Male - Caucasian 7",L"Male - Caucasian 8",L"Male - Caucasian 9",L"Male - Caucasian 10",L"Male - Caucasian 11",L"Male - Caucasian 12",L"Male - Caucasian 13",L"Male - Caucasian 14",L"Male - Caucasian 15",L"Male - Caucasian 16",L"Male - Caucasian 17",L"Male - Caucasian 18",L"Male - Caucasian 19",L"Male - Caucasian 20",L"Male - Caucasian 21",L"Male - Caucasian 22",L"Male - Caucasian 23",L"Male - Caucasian 24",L"Male - Caucasian 25",L"Male - Caucasian 26",L"Male - Caucasian 27",L"Male - Caucasian 28",L"Male - Caucasian 29",L"Male - Caucasian 30",L"Male - Caucasian 31",L"Male - Caucasian 32",L"Male - Caucasian 33",L"Male - Caucasian 34",L"Male - Caucasian 35",L"Male - Caucasian 36",L"Male - Caucasian 37",L"Male - Caucasian 38",L"Male - Caucasian 39",L"Male - Caucasian 40",L"Male - Caucasian 41",L"Male - Caucasian 42",L"Male - Caucasian 43",L"Male - Caucasian 44",L"Male - Caucasian 45",L"Male - Caucasian 46",L"Male - Caucasian 47",L"Male - Caucasian 48",L"Male - Caucasian 49",L"Male - Caucasian 50",L"Male - Caucasian 51",L"Male - Caucasian 52",L"Male - Caucasian 53",L"Male - Caucasian 54",L"Male - Caucasian 55",L"Male - Caucasian 56",L"Male - Asian 1",L"Male - Asian 2",L"Male - Asian 3",L"Male - Asian 4",L"Male - Asian 5",L"Male - Asian 6",L"Male - Asian 7",L"Male - Asian 8",L"Male - Asian 9",L"Male - Asian 10",L"Male - Asian 11",L"Male - Asian 12",L"Male - Asian 13",L"Male - Asian 14",L"Male - Asian 15",L"Male - Asian 16",L"Male - Asian 17",L"Male - Asian 18",L"Male - Asian 19",L"Male - Asian 20",L"Male - Asian 21",L"Male - Asian 22",L"Male - Asian 23",L"Male - Asian 24",L"Male - Asian 25",L"Male - Asian 26",L"Male - Asian 27",L"Male - Asian 28",L"Male - Asian 29",L"Male - Asian 30",L"Male - Asian 31",L"Male - Asian 32",L"Male - Asian 33",L"Male - Asian 34",L"Male - Asian 35",L"Male - Asian 36",L"Male - Asian 37",L"Male - Asian 38",L"Male - Asian 39",L"Male - Asian 40",L"Male - Asian 41",L"Male - Asian 42",L"Male - Asian 43",L"Male - Asian 44",L"Male - Asian 45",L"Male - Asian 46",L"Male - Asian 47",L"Male - Asian 48",L"Male - Asian 49",L"Male - Asian 50",L"Male - Asian 51",L"Male - Asian 52",L"Male - Asian 53",L"Male - Asian 54",L"Male - Asian 55",L"Male - Asian 56",L"Male - Asian 57",L"Male - Asian 58",L"Male - Asian 59",L"Male - Asian 60",L"Male - Asian 61",L"Male - Asian 62",L"Male - Asian 63",L"Male - Asian 64",L"Male - Asian 65",L"Male - Asian 66",L"Male - Asian 67",L"Male - Asian 68",L"Male - Asian 69",L"Male - Asian 70",L"Male - Arabic 1",L"Male - Arabic 2",L"Male - Arabic 3",L"Male - Arabic 4",L"Male - Arabic 5",L"Male - Arabic 6",L"Male - African 1",L"Male - African 2",L"Male - African 3",L"Male - African 4",L"Male - African 5",L"Male - African 6",L"Male - African 7",L"Male - African 8",L"Male - African 9",L"Male - African 10",L"Male - African 11",L"Male - African 12",L"Male - African 13",L"Male - African 14",L"Male - African 15",L"Male - African 16",L"Male - African 17",L"Male - African 18",L"Male - African 19",L"Male - African 20",L"Male - African 21",L"Male - African 22",L"Male - African 23",L"Male - African 24",L"Male - African 25",L"Male - African 26",L"Male - African 27",L"Male - African 28",L"Male - African 29",L"Male - African 30",L"Male - African 31",L"Male - African 32",L"Male - African 33",L"Male - African 34",L"Male - African 35",L"Male - African 36",L"Male - African 37",L"Male - African 38",L"Male - African 39",L"Male - African 40",L"Male - African 41",L"Male - African 42",L"Male - African 43",L"Male - African 44",L"Male - African 45",L"Male - African 46",L"Male - African 47",L"Male - African 48",L"Male - African 49",L"Male - Latin 1",L"Male - Latin 2",L"Male - Latin 3",L"Male - Latin 4",L"Male - Latin 5",L"Male - Latin 6",L"Male - Latin 7",L"Male - Latin 8",L"Male - Latin 9",L"Male - Latin 10",L"Male - Latin 11",L"Male - Latin 12",L"Male - Latin 13",L"Male - Latin 14",L"Male - Latin 15",L"Male - Latin 16",L"Male - Latin 17",L"Male - Latin 18",L"Male - Latin 19",L"Male - Latin 20",L"Male - Latin 21",L"Male - Latin 22",L"Male - Latin 23",L"Male - Latin 24",L"Male - Latin 25",L"Male - Latin 26",L"Male - Latin 27",L"Male - Latin 28",L"Male - Latin 29",L"Male - Latin 30",L"Male - Latin 31",L"Male - Latin 32",L"Male - Latin 33",L"Male - Latin 34",L"Male - Latin 35",L"Male - Latin 36",L"Male - Latin 37",L"Male - Latin 38",L"Male - Latin 39",L"Male - Latin 40",L"Male - Latin 41",L"Male - Latin 42",L"Male - Latin 43",L"Male - Latin 44",L"Male - Latin 45",L"Male - Latin 46",L"Male - Latin 47",L"Male - Latin 48",L"Male - Latin 49",L"Male - Latin 50",L"Male - Latin 51",L"Male - Latin 52",L"Male - East European 1",L"Male - East European 2",L"Male - East European 3",L"Male - East European 4",L"Male - East European 5",L"Male - East European 6",L"Male - East European 7",L"Male - East European 8",L"Male - East European 9",L"Male - East European 10",L"Male - East European 11",L"Male - East European 12",L"Male - East European 13",L"Male - East European 14",L"Male - East European 15",L"Male - East European 16",L"Male - East European 17",L"Male - East European 18",L"Male - East European 19",L"Male - East European 20",L"Male - East European 21",L"Male - East European 22",L"Male - East European 23",L"Male - East European 24",L"Male - East European 25",L"Male - East European 26",L"Male - East European 27",L"Male - East European 28",L"Male - East European 29",L"Male - East European 30",L"Male - Arabic 7",L"Male - Arabic 8",L"Male - Arabic 9",L"Male - Arabic 10",L"Male - Arabic 11",L"Male - Arabic 12",L"Male - Arabic 13",L"Male - Arabic 14",L"Male - Arabic 15",L"Male - Arabic 16",L"Male - Arabic 17",L"Male - Arabic 18",L"Male - Arabic 19",L"Male - Arabic 20",L"Male - Arabic 21",L"Male - Arabic 22",L"Male - Arabic 23",L"Male - Arabic 24",L"Male - Arabic 25",L"Male - Arabic 26",L"Male - Arabic 27",L"Male - Arabic 28",L"Male - African South 1",L"Male - African South 2",L"Male - African South 3",L"Male - African South 4",L"Male - African South 5",L"Male - African South 6",L"Male - South European 1",L"Male - South European 2",L"Male - South European 3",L"Male - South European 4",L"Male - South European 5",L"Male - South European 6",L"Male - South European 7",L"Male - South European 8",L"Male - South European 9",L"Male - South European 10",L"Male - South European 11",L"Male - South European 12",L"Male - South European 13",L"Male - South European 14",L"Male - South European 15",L"Male - South European 16",L"Male - South European 17",L"Male - South European 18",L"Male - North European 1",L"Male - North European 2",L"Male - North European 3",L"Male - North European 4",L"Male - North European 5",L"Male - North European 6",L"Male - North European 7",L"Male - North European 8",L"Male - North European 9",L"Male - North European 10",L"Male - North European 11",L"Male - North European 12",L"Male - North European 13",L"Male - North European 14",L"Male - North European 15",L"Male - North European 16",L"Male - North European 17",L"Male - North European 18",L"Male - North European 19",L"Male - North European 20",L"Male - North European 21",L"Male - African North 1",L"Male - African North 2",L"Male - African North 3",L"Male - African North 4",L"Male - African South European 1",L"Male - African South European 2",L"Male - African South European 3",L"Male - African South European 4",L"Female - Caucasian 1",L"Female - Caucasian 2",L"Female - Caucasian 3",L"Female - Caucasian 4",L"Female - Caucasian 5",L"Female - Caucasian 6",L"Female - Caucasian 7",L"Female - Caucasian 8",L"Female - Caucasian 9",L"Female - Caucasian 10",L"Female - Caucasian 11",L"Female - Caucasian 12",L"Female - Asian 1",L"Female - Asian 2",L"Female - Asian 3",L"Female - Asian 4",L"Female - Asian 5",L"Female - Asian 6",L"Female - Asian 7",L"Female - Asian 8",L"Female - Asian 9",L"Female - Asian 10",L"Female - Asian 11",L"Female - Asian 12",L"Female - Asian 13",L"Female - Asian 14",L"Female - Asian 15",L"Female - Asian 16",L"Female - Asian 17",L"Female - Asian 18",L"Female - Asian 19",L"Female - Asian 20",L"Female - Asian 21",L"Female - Asian 22",L"Female - Asian 23",L"Female - Asian 24",L"Female - Asian 25",L"Female - Asian 26",L"Female - Asian 27",L"Female - Asian 28",L"Female - Asian 29",L"Female - Asian 30",L"Female - African 1",L"Female - African 2",L"Female - African 3",L"Female - African 4",L"Female - African 5",L"Female - African 6",L"Female - African 7",L"Female - Latin 1",L"Female - Latin 2",L"Female - Latin 3",L"Female - Latin 4",L"Female - Latin 5",L"Female - Latin 6",L"Female - Latin 7",L"Female - Latin 8",L"Female - Latin 9",L"Female - Latin 10",L"Female - Latin 11",L"Female - Latin 12",L"Female - Latin 13",L"Female - Latin 14",L"Female - Latin 15",L"Female - Latin 16",L"Female - Latin 17",L"Female - Latin 18",L"Female - Latin 19",L"Female - Latin 20",L"Female - Latin 21",L"Female - Latin 22",L"Female - Latin 23",L"Female - Latin 24",L"Female - Latin 25",L"Female - Latin 26",L"Female - Latin 27",L"Female - Latin 28",L"Female - Latin 29",L"Female - Latin 30",L"Female - Latin 31",L"Female - Latin 32",L"Female - Latin 33",L"Female - Latin 34",L"Female - East European 1",L"Female - East European 2",L"Female - East European 3",L"Female - Arabic 1",L"Female - African South 1",L"Female - African South 2",L"Female - South European 1",L"Female - South European 2",L"Female - South European 3",L"Female - South European 4",L"Female - South European 5",L"Female - South European 6",L"Female - North European 1",L"Female - North European 2",L"Female - North European 3",L"Female - North European 4",L"Female - North European 5",L"Female - North European 6",L"Female - North European 7",L"Female - North European 8",L"Female - North European 9",L"Female - North European 10",L"Female - North European 11",L"Female - North European 12",L"Female - North European 13",L"Female - North European 14",L"Female - North European 15",L"Female - North European 16",L"Female - North European 17",L"Female - North European 18",L"Female - North European 19",L"Female - North European 20",L"Female - African North 1",L"Female - African North 2",L"Female - African North 3",L"Female - African North 4",L"Female - African North 5",L"Female - African North 6",L"Female - African North 7",L"Female - African North 8",L"Female - African North 9",L"Female - African North 10",L"Female - African North 11",L"Female - African North 12",L"Female - African North 13",L"Female - African North 14",L"Female - African North 15",L"Female - African North 16",L"Female - African North 17",L"Female - African North 18",L"Female - African North 19",L"Female - African North 20",L"Female - African North 21",L"Female - African North 22",L"Female - African North 23",L"Female - African North 24",L"Female - African North 25",L"Female - African North 26",L"Female - African North 27",L"Female - African South European 1",L"Female - African South European 2",L"Female - African South European 3",L"Female - African South European 4",L"Female - African South European 5",L"Female - African South European 6",L"Female - African South European 7",L"Female - African South European 8"
};
Vector<String> AppearanceGenerator::HairNames = {
    L"Bald (0)",L"Afro Shaved 1 (1)",L"Afro Short Messy (2)",L"Afro Large 1 (3)",L"Afro Corn Rows 1 (4)",L"Afro Dreads Long 1 (5)",L"Afro Dreads Ponytail 1 (6)",L"Ponytail (7)",L"Medium Slick 1 (8)",L"Long 2 (9)",L"Long 1 (10)",L"Long Curly 1 (11)",L"Long 4 (12)",L"Medium Straight 1 (13)",L"Medium Curly 1 (14)",L"Mullet (15)",L"Short Clean 1 (16)",L"Short Spiked Front 1 (17)",L"Short Sidewards Spikey 1 (18)",L"Short Messy 1 (19)",L"Medium Fringe 1 (20)",L"Short Curly 1 (21)",L"Short Curly 2 (22)",L"Short Caesar 1 (23)",L"Short 1 (24)",L"Shaved 1 (25)",L"Balding Top 1 (26)",L"Afro Dreads Messy (27)",L"Afro Short Clean 1 (28)",L"Balding Front 1 (29)",L"Short Slick 1 (30)",L"Short Curly Wet Receding (31)",L"Long Bangs (32)",L"Long Straight (33)",L"Long Flick (34)",L"Medium Bob 1 (35)",L"Short Slick Shaved (36)",L"Short Receding 1 (37)",L"Short Slicked Forward (38)",L"Short Spiked Front 2 (39)",L"Short Fringe 1 (40)",L"Balding Top 2 (41)",L"Short Straight 2a (42)",L"Shaved Receding (43)",L"Medium Curly Mullet (44)",L"Afro Short 1 (45)",L"Afro Shaved Receding (46)",L"Afro Short Receding 1 (47)",L"Ponytail Thick Headband (48)",L"Long Curly Headband 2 (49)",L"Long Curly 3 (51)",L"Medium Curly 2 (52)",L"Bun Small 1 (53)",L"Short Clean Spikey 1 (54)",L"Medium Straight Headband 1 (55)",L"Long Headband Thin (56)",L"Short Sidewards 1 (57)",L"Medium Bangs 2 (58)",L"Medium Straight 2 (59)",L"Afro Short 2 (60)",L"Short Mohawk 1 (61)",L"Medium Fringe 2 (62)",L"Short Mohawk Shaved 1 (63)",L"Short Spikey 1 (64)",L"Short Clean 2a (65)",L"Short Flick 1 (66)",L"Short Flick 2 (67)",L"Medium Bob 3 (68)",L"Medium Bob 2 (69)",L"Short Spikey Messy (70)",L"Afro Corn Rows Short (71)",L"Short Receding 2 (72)",L"Medium Flick 1 (73)",L"Medium Straight Bob (74)",L"Short Clean Sidewards 1 (75)",L"Long Curly Headband 1 (76)",L"Short 2 (77)",L"Short Straight 1 (78)",L"Long Curly 4 (79)",L"Long Wavy (80)",L"Long Wavy Headband (81)",L"Afro Short Shaved Sides 1 (82)",L"Medium Bangs 1 (83)",L"Long 3 (84)",L"Short Middle Part (85)",L"Short Mohawk 2 (86)",L"Medium Curly 3 (87)",L"Short Spikey Shaved Sides 1 (88)",L"Short Messy Wet 1 (89)",L"Short Sidewards Receding 1 (90)",L"Long Headband Thick 1 (91)",L"Balding Front 2 (92)",L"Short Wavy 1 (93)",L"Medium Puffy 1 (94)",L"Mullet Curly Bangs (95)",L"Afro Medium 1 (96)",L"Afro Medium 2 (97)",L"Medium Bangs 4 (98)",L"Medium Curly Headband (99)",L"Short Messy Receding (100)",L"Short Messy 2 (101)",L"Short Middle Thing (102)",L"Medium Curly 4 (103)",L"Short Bangs 1 (104)",L"Short Wavy 5 (105)",L"Short Curly 3a (106)",L"Short Fringe 2 (107)",L"Medium Straight Bangs (108)",L"Afro Dreads Medium 1 (109)",L"Afro Dreads Medium 2 (110)",L"Short Bangs Bob (111)",L"Medium Receding 1 (112)",L"Afro Short Curls (113)",L"Short Straight Receding 1 (114)",L"Short Clean Sidewards 2 (115)",L"Short Fringe Wavy (116)",L"Short Clean Receding 1 (117)",L"Short Sidewards 2a (118)",L"Short Straight 3 (119)",L"Afro Mohawk Small (120)",L"Short Fringe 3 (121)",L"Short Receding Messy 1 (122)",L"Short Sidewards Shaved 1 (123)",L"Short Sides Receding (124)",L"Long Caesar (125)",L"Medium Spikey 1 (126)",L"Short Caesar 2 (127)",L"Medium Puffy 2 (128)",L"Short Spikey 2 (129)",L"Medium Sidewards Spikey 1 (130)",L"Short Fringe 4 (131)",L"Afro Mohawk (132)",L"Short Sidewards 3 (133)",L"Short Receding Spikey 1 (134)",L"Short Receding Modern 1 (135)",L"Medium Bangs 5 (136)",L"Short Clean Sidewards 3 (137)",L"Medium Spikey Front (138)",L"Medium Receding 2 (139)",L"Short Sidewards Slick 1 (140)",L"Short 3 (141)",L"Medium Bangs 6 (142)",L"Short Sides Fringe 1 (143)",L"Short Curly Wet (144)",L"Short Caesar 3 (145)",L"Short Messy Wet 2 (146)",L"Short Curly Messy 2 (147)",L"Short Clean Spikey 2 (148)",L"Short Straight 4 (149)",L"Short Receding 3 (150)",L"Short Straight 5 (151)",L"Short Sides Fringe 2 (152)",L"Medium Caesar 1 (153)",L"Short Clean Receding 2 (154)",L"Short Modern (155)",L"Short Messy 3 (156)",L"Medium Headband (157)",L"Short Fringe Messy 1 (158)",L"Medium Receding Slick (159)",L"Short Clean Sidewards 4 (160)",L"Short Clean 3 (161)",L"Medium Slick 2 (162)",L"Medium Sidewards Shaved (163)",L"Short Receding Spikey 2 (164)",L"Medium Sidewards 4 (165)",L"Short Clean Sidewards 5a (166)",L"Short Receding 4 (167)",L"Medium Receding 3 (168)",L"Short Sidewards Shaved 2 (169)",L"Short Sidewards 6 (170)",L"Medium Slick Shaved (171)",L"Short Sidewards Slick 2 (172)",L"Short Mohawk Slick (173)",L"Short Straight 6 (174)",L"Short Clean Spikey 3 (175)",L"Short 4 (176)",L"Short Wavy 2a (177)",L"Medium Sidewards Slick 1 (178)",L"Short Sidewards 2 Flipped (179)",L"Medium Wavy 1 (180)",L"Short Clean Receding 3 (181)",L"Short Receding 5 (182)",L"Medium Slick 3 (183)",L"Short Sidewards 4b (184)",L"Short Mohawk Modern (185)",L"Short Receding Spikey 3 (186)",L"Short Sidewards Slick 3 (187)",L"Short Receding Modern 2 (188)",L"Short 5 (189)",L"Short Spiked Front 3 (190)",L"Short Sidewards 7 (191)",L"Short Clean Sidewards 6 (192)",L"Short Sidewards Receding 2 (193)",L"Afro Mohawk Shaved (194)",L"Short Straight 7 (195)",L"Afro Medium 6 (196)",L"Afro Short Shaved Sides 2 (197)",L"Short Sidewards Wavy 1 (198)",L"Short Slick Backwards 1 (199)",L"Short Spikey Shaved Sides 2 (200)",L"Afro Short Curly (201)",L"Afro Curly Receding (202)",L"Special (203)",L"Short Curly Tight (204)",L"Short Wavy 3 (205)",L"Shaved 2 (206)",L"Short Curly Messy 1 (207)",L"Short Sidewards Clean 2 (208)",L"Medium Slicked Back 1 (209)",L"Medium Slicked Back 3 (210)",L"Medium Sidewards Messy 1 (211)",L"Short Slicked Back (212)",L"Short Clean Sidewards 5b (218)",L"Short Messy 4a (219)",L"Short Shaved Sides 1 (220)",L"Short Spikey Straight 1 (221)",L"Short Messy 5 (222)",L"Short Sidewards Slick 4 (223)",L"Afro Large 3 (224)",L"Short Curly Shaved Sides 1 (225)",L"Afro Dreads Shaved Sides 2 (226)",L"Shaved 3 (227)",L"Afro Corn Rows 3 (228)",L"Short Sidewards Receding 3 (229)",L"Short Sidewards Clean 1 (230)",L"Short Sidewards Clean 3 (231)",L"Short Shaved Sides 3 (232)",L"Afro Dreads Modern (233)",L"Short Sidewards 8a (234)",L"Short Messy 6 (235)",L"Short Curly Shaved Sides 2 (236)",L"Short Messy High (237)",L"Medium Middle Part 2 (238)",L"Afro Short 6 (239)",L"Afro Short Clean Shaved (240)",L"Afro Short Shaved Sides 3 (241)",L"Short Slick Backwards 2 (242)",L"Short Clean 4 (243)",L"Short Wavy Receding (244)",L"Short Flick Shaved Sides 2 (245)",L"Afro Dreads Medium 3 (246)",L"Afro Short 5 (250)",L"Medium Middle Part 3 (251)",L"Special (253)",L"Double Buns 1 (255)",L"Double Buns 2 (256)",L"Medium Messy (257)",L"Afro Dreads Medium 4 (258)",L"Ponytail Braid Small 2 (260)",L"Medium Spikey 2 (261)",L"Afro Dreads Shaved Sides 4 (262)",L"Medium Fringe 3 (263)",L"Bun Big Shaved Sides (264)",L"Bun Braids (265)",L"Afro Bantu Knots (266)",L"Afro Dreads Shaved Sides 1 (267)",L"Bun Asian (268)",L"Bun Afro Big 3 (269)",L"Afro Mohawk Big 1 (270)",L"Bun Afro Big 4 (271)",L"Afro Mohawk Messy (272)",L"Afro Corn Rows Long 1 (273)",L"Afro Dreads Ponytail 2 (274)",L"Afro Medium 4 (275)",L"Afro Large 2 (276)",L"Medium Curly 5 (277)",L"Afro Slick (278)",L"Ponytail Up Long 4a (279)",L"Afro Corn Rows 2 (280)",L"Bun Medium 1 (281)",L"Medium Fringe 4 (282)",L"Short Spikey 3 (283)",L"Short Sidewards 4a (284)",L"Short Mohawk Shaved 2 (285)",L"Ponytail Medium 3 (286)",L"Medium Slicked Back 4 (288)",L"Short Sidewards 2b (289)",L"Balding Front 4 (290)",L"Short Fringe 5 (291)",L"Short Sidewards 9a (292)",L"Short Receding Messy 2 (293)",L"Short Curly 4a (294)",L"Short Sidewards 5 (295)",L"Afro Dreads Long Shaved (296)",L"Balding Front 3 (297)",L"Short Flick Shaved Sides 1 (298)",L"Afro Medium 5 (299)",L"Medium Mop Bangs (374)",L"Ponytail Fringe Shaved Sides (375)",L"Bun Shaved (376)",L"Loose Hair Long Messy (381)",L"Long Curly 2 (385)",L"Afro Corn Rows Medium 2 (386)",L"Special (389)",L"Afro Short Straight Line (390)",L"Loose Hair Long (392)",L"Medium Flick Shaved Messy (394)",L"Afro Mohawk Big 2 (398)",L"Afro Mohawk Big 3 (399)",L"Loose Hair Bangs Fringe (418)",L"Afro Dreads Short Sides (419)",L"Loose Hair Medium 2 (421)",L"Medium Flick 2 (431)",L"Ponytail Long 2 (432)",L"Bun Medium 2 (433)",L"Afro Corn Rows Medium 1 (434)",L"Ponytail Braid Long 1 (435)",L"Ponytail Up Big (436)",L"Ponytail Long Fringe (437)",L"Ponytail Low Fringe (438)",L"Loose Hair Medium 1b (444)",L"Short Sidewards Clean 4 (445)",L"Medium Curly Shaved Sides 1 (446)",L"Mullet w/ Short Bangs (447)",L"Loose Hair Medium 3 (448)",L"Medium Bangs 3 (450)",L"Afro Short Clean 2 (451)",L"Ponytail Wrapped 3 (452)",L"Afro Corn Rows Long 2 (460)",L"Nest Curly Big (461)",L"Medium Flick Curly (462)",L"Medium Flick Shaved (463)",L"Afro Dreads Up 2 (492)",L"Double Buns 3 (494)",L"Bun (500)",L"Ponytail Long Headband 1 (501)",L"Bob Medium Headband (502)",L"Ponytail Braid 1 (503)",L"Ponytail Short Headband (504)",L"Ponytail Up Long 1 (505)",L"Bun Afro 1 (506)",L"Ponytail Wrapped Up Headband (507)",L"Ponytail Up Long Headband (508)",L"Short Spikey 4 (509)",L"Ponytail Long 1 (510)",L"Ponytail Fringe (511)",L"Ponytail Medium 1 (512)",L"Medium Straight Headband 2 (513)",L"Bob Medium (514)",L"Ponytail Short Messy (515)",L"Bob Short (516)",L"Ponytail Wrapped 1 (517)",L"Ponytail Afro (518)",L"Ponytail Medium Headband 1 (519)",L"Bob Short Thin Headband (520)",L"Short Straight Headband (521)",L"Medium Mop 1 (522)",L"Long Headband Thick 2 (523)",L"Ponytail Curly 1 (524)",L"Ponytail Dreads (525)",L"Ponytail Braid 2 (526)",L"Bob Short Thick Headband (527)",L"Nest Headband (528)",L"Short Sidewards Fem (529)",L"Ponytail Curly 2 (530)",L"Ponytail Curly Low (531)",L"Ponytail Braid 3 (532)",L"Bun Curly Thick Headband (533)",L"Bun Small Tight Up (534)",L"Ponytail Straight Low 2 (535)",L"Ponytail Medium 2a (536)",L"Ponytail Wrapped Small (537)",L"Ponytail Short (538)",L"Ponytail Up Long 2 (539)",L"Bun Small Headband (540)",L"Medium Slick Messy (541)",L"Shaved 5 (542)",L"Afro Short 3 (543)",L"Afro Short 4 (544)",L"Afro Short Shaved Sides 4 (545)",L"Ponytail Dreads Medium (546)",L"Ponytail Dreads Long (547)",L"Afro Medium 3 (548)",L"Afro Dreads Medium 3a (549)",L"Bun Afro Clean (550)",L"Ponytail Dreads Short (551)",L"Medium Middle Part 1 (552)",L"Medium Mop 2 (553)",L"Short Sidewards Spikey 2 (554)",L"Medium Sidewards 3 (555)",L"Medium Caesar 2 (556)",L"Medium Mid-Part Headband (557)",L"Medium Sidewards 2 (558)",L"Ponytail Braid Small 1 (559)",L"Ponytail Up Medium Headband 1 (560)",L"Ponytail Wide Headband (561)",L"Bun Braid Headband (562)",L"Ponytail Up Long 3 (563)",L"Ponytail Curly Long Headband (564)",L"Nest Messy (565)",L"Ponytail Long Headband Pink (566)",L"Ponytail Long Headband Blue (567)",L"Ponytail Medium Headband Pink (568)",L"Ponytail Medium Headband Blue (569)",L"Ponytail Short Headband Pink (570)",L"Ponytail Short Headband Blue (571)",L"Short Sidewards High 1 (572)",L"Short Slick Headband (573)",L"Short Sidewards Messy 1 (574)",L"Short Shaved Sides 2 (575)",L"Bun Afro 2 (576)",L"Medium Curly Dreads (577)",L"Nest Curly (578)",L"Ponytail Wrapped 2 (579)",L"Medium Sidewards 1 (580)",L"Ponytail Wide Up Band (581)",L"Ponytail Medium 2b (582)",L"Ponytail Up Long 4b (583)",L"Ponytail Up Long Big (584)",L"Loose Hair Very Long 1 (585)",L"Loose Hair Long Curly (586)",L"Ponytail Up Bangs (587)",L"Loose Hair Medium 1a (588)",L"Loose Hair Long Wavy 1 (589)",L"Loose Hair Very Long 2 (590)",L"Loose Hair Very Long 3 (591)",L"Loose Hair Very Long 4 (592)",L"Loose Hair Very Long Wavy (593)",L"Loose Hair Long Wavy 2 (594)",L"Ponytail Medium 5 (595)",L"Ponytail Medium 4 (596)",L"Bun Small Tight 1 (597)",L"Medium Curly Back (598)",L"Ponytail Long Headband 2 (599)",L"Ponytail Up Very Long 1 (600)",L"Nest Thick Headband (601)",L"Afro Dreads Up 1 (602)",L"Ponytail Straight Low 1 (603)",L"Bun Up Tight (604)",L"Afro Big Curly (605)",L"Bun Double (606)",L"Bun Up 1 (607)",L"Bun Donut 2 (608)",L"Ponytail Medium Headband 2 (609)",L"Bun Donut Small Up (610)",L"Afro Dreads Long Low (611)",L"Ponytail Medium Up Wavy (612)",L"Bun Afro Big 1 (613)",L"Bun Medium 3 (614)",L"Bun Tight Headband (615)",L"Ponytail Up Short (616)",L"Bun Small 3 (617)",L"Bun Big Donut (618)",L"Afro Dreads Long Ponytail (619)",L"Bun Low Braids (620)",L"Afro Dreads Ponytail Up (621)",L"Ponytail Up Medium Headband 3 (622)",L"Bun Donut 1 (623)",L"Bun Donut Rubber Band (624)",L"Nest Messy Up (625)",L"Special (626)",L"Bun Up Loose 1 (627)",L"Bun Braid Medium (628)",L"Bun Up 5 (629)",L"Medium Swept Back (630)",L"Short Wavy 4 (631)",L"Short Straight Receding 2a (632)",L"Short Clean Slick (633)",L"Short Wavy 2b (634)",L"Short Sidewards Hairline (635)",L"Short Clean 2b (636)",L"Medium (637)",L"Short Spikey Straight 2 (638)",L"Medium Clean (639)",L"Short Sidewards High 2 (640)",L"Short Sidewards Receding 4 (641)",L"Short Sidewards Receding 5 (642)",L"Short Curly 5 (643)",L"Short Straight 2b (644)",L"Short Messy Wet 3 (645)",L"Short Clean Slick Flipped 1 (646)",L"Bun Small Low (647)",L"Short Curly 6 (648)",L"Short Sidewards 9b (649)",L"Afro Short 7 (650)",L"Short Mohawk Short Sides (651)",L"Short Clean Slick Flipped 2 (652)",L"Medium Slicked Back 2 (653)",L"Medium Receding Spikey (654)",L"Short Straight Receding 3 (655)",L"Short Clean Short Sides (656)",L"Short Spikey Straight 3 (657)",L"Short Curly 4b (659)",L"Short Curly Short Sides (660)",L"Medium Sidewards Messy 2 (661)",L"Short 6 (662)",L"Medium Sidewards Slick 2 (663)",L"Afro Dreads Medium 3b (664)",L"Short Shaved (665)",L"Medium Sidewards 5 (666)",L"Medium Sidewards Fringe (667)",L"Short Messy Sidewards (668)",L"Short Sidewards Short Sides (669)",L"Medium Receding Messy (670)",L"Short Messy 4b (671)",L"Short Straight Line (672)",L"Short Slick 2 (673)",L"Short Clean Spikey 5 (674)",L"Afro Short Receding 2 (675)",L"Short Clean Spikey 4 (676)",L"Short Curly Fringe (677)",L"Medium Sidewards 6 (678)",L"Short Sidewards Wavy 2 (679)",L"Afro Corn Rows 4 (680)",L"Short Shaved Sides Receding (681)",L"Short Spikey Sideburns (682)",L"Short Curly Shaved Sides 3 (683)",L"Loose Hair Very Long 5 (684)",L"Short Spikey Shaved Sides 3 (685)",L"Short Sidewards Modern 1 (686)",L"Short Curly 7a (687)",L"Short Sidewards Messy 2 (688)",L"Short Curly 3b (689)",L"Short Curly 7b (690)",L"Afro Short 8 (691)",L"Short Straight Receding 2b (692)",L"Short Receding Modern 3 (693)",L"Medium Slick 4 (694)",L"Shaved 4 (695)",L"Medium Receding 4 (696)",L"Short Curly 3c (697)",L"Short Clean Shaved Sides (698)",L"Ponytail Braid Up Headband (720)",L"Bun Medium Loose Sides (721)",L"Ponytail Medium 2c (722)",L"Ponytail Braid Low (723)",L"Ponytail Long Wrapped (724)",L"Bun Up Double Headband (725)",L"Crown Braid 2 (726)",L"Bun Afro Big 2 (727)",L"Bun Afro Back (728)",L"Ponytail Medium Bangs (729)",L"Bun Fringe (730)",L"Medium Curly Afro Headband (731)",L"Bun Donut Hairline Messy (732)",L"Ponytail Braid Long Thin (733)",L"Bun Afro Dreads (734)",L"Ponytail Small Low Fringe (735)",L"Ponytail Low Sleek 1 (736)",L"Ponytail Low Sleek 2 (737)",L"Bun Double Afro (738)",L"Ponytail Low Wide Afro (739)",L"Ponytail Up Medium Messy (740)",L"Ponytail Long Low (741)",L"Ponytail Braid Thin Headband (742)",L"Ponytail Long Wide Headband (743)",L"Ponytail Long Wide (744)",L"Ponytail Wide Headband Thick (745)",L"Ponytail Wide Headband Blue (746)",L"Nest Afro Dreads (747)",L"Ponytail Braid Headband Thick (748)",L"Ponytail Long Afro Dreads (749)",L"Special (810)",L"Special (811)",L"Bun Afro 4 (815)",L"Afro Curly Ponytail Big (816)",L"Medium Fringe 5 (828)",L"Loose Hair Fringe (831)",L"Medium Fringe 6 (832)",L"Special (845)",L"Fringe Headband Blue (881)",L"Special (890)",L"Ponytail Braid Long 2 (891)",L"Ponytail Up Very Long 2 (892)",L"Bun Headband (900)",L"Ponytail Braid Small 3 (901)",L"Short Sidewards Shaved Sides (902)",L"Bun Small Up 1 (903)",L"Bun Small Up 2 (904)",L"Bun Up 2 (905)",L"Bun Afro 3 (906)",L"Afro Big Curly Headband (907)",L"Bun Up Medium (908)",L"Bun Tight Hairline (909)",L"Bun Braid (910)",L"Ponytail Up Curly (911)",L"Bun Up Loose 2 (912)",L"Bun Big 1 (913)",L"Bun Low Left Hairline (914)",L"Bun Small Tight 2 (915)",L"Bun Low Mid Hairline (916)",L"Bun Up 3 (917)",L"Bun Big 2 (918)",L"Bun Up Loose 3 (919)",L"Bun Small Up 3 (920)",L"Bun Small Scrunchie (921)",L"Bun Up 4 (922)",L"Ponytail Wrapped Up (923)",L"Crown Braid 1 (924)",L"Bun Small 2 (925)",L"Afro Curly Medium Ponytail (926)",L"Ponytail Up Medium Headband 2 (927)",L"Bun Low Curly Hairline (928)",L"Ponytail Braid Small 4 (930)",L"Ponytail Braid 5 (931)",L"Ponytail Braid Long 3 (932)",L"Special (1059)",L"Special (1060)",L"Medium Curly Shaved Sides 2 (1061)",L"Short Messy Shaved Sides (1062)",L"Bowl Cut 1 (1063)",L"Short Fringe Shaved Messy (1064)",L"Short Sides (1065)",L"Afro Medium Shaved Sides 1 (1066)",L"Medium Sidewards Spikey Front (1067)",L"Short Messy Short Sides 1 (1068)",L"Short Bangs Flipped (1069)",L"Medium Curly Short Sides (1070)",L"Short Sidewards 4 Receding (1071)",L"Short Fringe Shaved Sides 1 (1072)",L"Short Fringe Short Sides (1073)",L"Short Messy Wet 4 (1074)",L"Short Sidewards 8b (1075)",L"Afro Dreads Medium 3c (1076)",L"Short Messy Short Sides 2 (1077)",L"Short Fringe Shaved Sides 2 (1078)",L"Short Slick Black Headband (1079)",L"Short Fringe Shaved Sides 3 (1080)",L"Afro Medium 7 (1081)",L"Afro Short Shaved Sides 5 (1082)",L"Short Fringe Messy Forehead (1083)",L"Afro Curly Short Sides (1084)",L"Short Sidewards Messy 3 (1085)",L"Afro Medium Shaved Sides 2 (1086)",L"Afro Dreads Shaved Sides 3 (1087)",L"Short Mohawk Shaved Sides (1088)",L"Medium Wavy 2 (1089)",L"Bowl Cut Fringe (1090)",L"Short Slick Back Headband (1091)",L"Short Shaved Sides Modern (1092)",L"Medium Sidewards Messy 3 (1093)",L"Special (1094)",L"Special (1095)",L"Special (1096)",L"Special (1099)",L"Short 7 (1106)",L"Special (1157)",L"Short 8 (1281)",L"Medium Receding 5 (1282)",L"Short Receding 6 (1283)",L"Short Clean Slick Flipped 3 (1284)",L"Bowl Cut 2 (1285)",L"Special (1316)",L"Afro Mohawk Big 4 (1317)",L"Special (1337)",L"Special (1338)",L"Special (1341)",L"Special (1368)",L"Special (1370)",L"Special (1373)",L"Special (1374)",L"Special (1377)",L"Special (1378)",L"Special (1384)",L"Special (1385)",L"Special (1386)",L"Special (2000)",L"Special (2001)",L"Special (2002)",L"Special (2003)",L"Special (2004)",L"Special (2005)",L"Special (2006)",L"Special (2007)",L"Special (2008)",L"Special (2009)",L"Special (2010)",L"Special (2011)",L"Special (2012)",L"Special (2013)",L"Special (2014)",L"Special (2015)",L"Special (2016)",L"Special (2017)",L"Special (2018)",L"Special (2019)",L"Special (2020)",L"Special (2021)",L"Special (2022)",L"Special (2023)",L"Special (2024)",L"Special (2025)",L"Special (2026)",L"Special (2027)",L"Special (2028)",L"Special (2029)",L"Special (2030)",L"Special (2031)",L"Special (2032)",L"Special (2033)",L"Special (2034)",L"Special (2035)",L"Special (2036)",L"Special (2037)",L"Special (2038)",L"Special (2039)",L"Special (2040)",L"Special (2041)",L"Special (2042)",L"Special (2043)",L"Special (2044)",L"Special (2045)",L"Special (2046)",L"Special (2047)",L"Special (2048)",L"Special (2049)",L"Special (2050)",L"Special (2051)",L"Special (2052)",L"Special (2053)",L"Special (2054)",L"Special (2055)",L"Special (2056)",L"Special (2057)",L"Special (2058)",L"Special (2059)",L"Special (2060)",L"Special (2061)",L"Special (2062)",L"Special (2063)",L"Special (2064)",L"Special (2065)",L"Special (2066)",L"Special (2067)",L"Special (2068)",L"Special (3000)",L"Special (3001)",L"Special (3002)",L"Special (3003)",L"Special (3004)",L"Special (3005)",L"Special (3006)",L"Special (3007)",L"Special (3008)",L"Special (3009)",L"Special (3010)",L"Special (3011)",L"Special (3012)",L"Special (3013)",L"Special (3014)",L"Special (3015)",L"Special (3016)",L"Special (3017)",L"Special (3018)",L"Special (3019)",L"Special (3020)",L"Special (3021)",L"Special (3022)",L"Special (3025)",L"Special (3026)",L"Special (3027)",L"Special (3028)",L"Special (3029)",L"Special (3030)",L"Special (3031)",L"Special (3032)",L"Special (3033)",L"Special (3034)",L"Special (3035)",L"Special (3036)",L"Special (3037)",L"Special (3038)",L"Special (3039)",L"Special (3040)",L"Special (3041)",L"Special (3042)",L"Special (3043)",L"Special (3044)",L"Special (3045)",L"Special (3046)",L"Special (3047)",L"Special (3048)",L"Special (3049)",L"Special (3050)",L"Special (3051)",L"Special (3052)",L"Special (3053)",L"Special (3055)",L"Special (3056)",L"Special (3057)",L"Special (3058)",L"Special (3059)",L"Special (3060)",L"Special (3061)",L"Special (3062)",L"Special (3063)",L"Special (3064)",L"Special (3065)",L"Special (3066)",L"Special (3067)",L"Special (3068)",L"Special (3069)",L"Special (3070)",L"Special (3071)",L"Special (3072)",L"Special (3073)",L"Special (3074)",L"Special (3075)",L"Special (3076)",L"Special (3077)",L"Special (3078)",L"Special (3079)",L"Special (3080)",L"Special (3081)",L"Special (3082)",L"Special (3083)",L"Special (3084)",L"Special (3085)",L"Afro Corn Rows 5 (3086)",L"Special (3087)",L"Special (3088)",L"Special (3089)",L"Special (3090)",L"Special (3091)",L"Special (3092)",L"Special (3093)",L"Special (3094)",L"Special (3095)",L"Special (3096)",L"Special (3097)",L"Special (3098)",L"Special (3100)",L"Special (3101)",L"Special (3102)",L"Special (3103)",L"Special (3104)",L"Special (3105)",L"Special (3106)",L"Special (3107)",L"Special (3108)",L"Special (3109)",L"Special (3110)",L"Special (3111)",L"Special (3112)",L"Special (3113)",L"Special (3114)",L"Special (3115)",L"Special (3116)",L"Special (3117)",L"Special (3118)",L"Special (3119)",L"Special (3120)",L"Special (3121)",L"Special (3122)",L"Special (3123)",L"Special (3124)",L"Special (3125)",L"Special (3126)",L"Special (3127)",L"Special (3128)",L"Special (3129)",L"Special (3130)",L"Special (3131)",L"Special (3132)",L"Special (3133)",L"Special (3134)",L"Special (3135)",L"Long 5 (3136)",L"Special (3137)",L"Special (3138)",L"Special (3139)",L"Special (3140)",L"Special (3141)",L"Special (3142)",L"Special (3143)",L"Special (3144)",L"Special (3145)",L"Special (3146)",L"Special (3147)",L"Special (3148)",L"Special (3149)",L"Special (3150)",L"Special (3151)",L"Special (3152)",L"Special (3153)",L"Special (3154)",L"Special (3155)",L"Special (3156)",L"Special (3157)",L"Special (3158)",L"Special (3159)",L"Special (3160)",L"Special (3161)",L"Special (3162)",L"Special (3163)",L"Special (3164)",L"Special (3165)",L"Special (3166)",L"Special (3167)",L"Special (3168)",L"Special (3169)",L"Special (3170)",L"Special (3171)",L"Special (3172)",L"Special (3173)",L"Special (3174)",L"Special (3175)",L"Special (3176)",L"Special (3177)",L"Special (3178)",L"Special (3179)",L"Special (3180)",L"Special (3181)",L"Special (3182)",L"Special (3183)",L"Special (3184)",L"Special (3185)",L"Special (3186)",L"Special (3187)",L"Special (3188)",L"Special (3189)",L"Special (3190)",L"Special (3191)",L"Special (3192)",L"Special (3193)",L"Special (3194)",L"Special (3195)",L"Special (3196)",L"Special (3197)",L"Special (3198)",L"Special (3199)",L"Special (3200)",L"Special (3201)",L"Special (3202)",L"Special (3203)",L"Special (3204)",L"Special (3205)",L"Special (3206)",L"Special (3207)",L"Special (3208)",L"Special (3209)",L"Special (3210)",L"Special (3211)",L"Special (3212)",L"Special (3213)",L"Special (3214)",L"Special (3215)",L"Special (3216)",L"Special (3217)",L"Special (3218)",L"Special (3219)",L"Special (3220)",L"Special (3221)",L"Special (3232)",L"Special (3233)",L"Special (3234)",L"Special (3237)",L"Special (3238)",L"Special (3239)",L"Special (3240)",L"Special (3241)",L"Special (3242)",L"Special (3244)",L"Special (3245)",L"Special (3246)",L"Special (3247)",L"Special (3248)",L"Special (3249)",L"Special (3250)",L"Special (3251)",L"Special (3252)",L"Special (3253)",L"Special (3254)",L"Special (3255)",L"Special (3256)",L"Special (3257)",L"Special (3258)",L"Special (3259)",L"Special (3260)",L"Special (3261)",L"Special (3262)",L"Special (3263)",L"Special (3264)",L"Special (3265)",L"Special (3266)",L"Special (3267)",L"Special (3268)",L"Special (3269)",L"Special (3270)",L"Special (3271)",L"Special (3272)",L"Special (3273)",L"Special (3274)",L"Special (3275)",L"Special (3276)",L"Special (3277)",L"Special (3278)",L"Special (3279)",L"Special (3280)",L"Special (3281)",L"Special (3282)",L"Special (3283)",L"Special (3284)",L"Special (3285)",L"Special (3298)",L"Special (3300)",L"Special (4000)",L"Special (4001)",L"Special (4002)",L"Special (4003)",L"Special (4004)",L"Special (4005)",L"Special (4006)",L"Special (4007)",L"Special (4008)",L"Special (4009)",L"Special (4010)",L"Special (4011)",L"Special (4012)",L"Special (4013)",L"Special (4014)",L"Special (4015)",L"Special (4016)",L"Special (4017)",L"Special (4018)",L"Special (4019)",L"Special (4020)",L"Special (4021)",L"Special (4022)",L"Special (4023)",L"Special (4024)",L"Special (4025)",L"Special (4026)",L"Special (4027)",L"Special (4028)",L"Special (4029)",L"Special (4030)",L"Special (5000)",L"Special (5001)",L"Special (5002)",L"Special (5003)",L"Special (5004)",L"Special (5005)",L"Special (5006)",L"Special (5007)",L"Special (5008)",L"Special (5009)",L"Special (5010)",L"Special (5011)",L"Special (5012)",L"Special (5013)",L"Special (5014)",L"Special (5015)",L"Special (5016)",L"Special (5017)",L"Special (6000)",L"Special (6001)",L"Special (6002)",L"Special (6003)",L"Special (6004)",L"Special (6005)",L"Special (6006)",L"Special (6007)",L"Special (6008)",L"Special (6009)",L"Special (6010)",L"Special (6011)",L"Special (6012)",L"Special (6013)",L"Special (6014)",L"Special (6015)",L"Special (6016)",L"Special (6017)",L"Special (6018)",L"Special (6019)",L"Special (6020)",L"Special (6021)",L"Special (6022)",L"Special (6023)",L"Special (6024)",L"Special (6025)",L"Special (6026)",L"Special (6033)",L"Special (6035)",L"Special (6036)",L"Special (6037)",L"Special (6038)",L"Special (6039)",L"Special (6040)",L"Special (6041)",L"Special (6042)",L"Special (7007)",L"Special (7010)",L"Special (7013)",L"Special (7014)",L"Special (7018)",L"Special (7021)",L"Special (7025)",L"Special (7027)",L"Special (7034)",L"Special (7036)",L"Special (7037)",L"Special (7038)",L"Special (7045)",L"Special (7046)",L"Special (7047)",L"Special (7050)",L"Special (7051)",L"Special (7052)",L"Special (7054)",L"Special (7056)",L"Special (7057)",L"Special (7060)",L"Special (7070)",L"Special (7072)",L"Special (7080)",L"Special (7082)",L"Special (7083)",L"Special (7087)",L"Special (7095)",L"Special (7103)",L"Special (7117)",L"Special (7129)",L"Special (7136)",L"Special (7140)",L"Special (7145)",L"Special (7147)",L"Special (7155)",L"Special (7161)",L"Special (7163)",L"Special (7167)",L"Special (7181)",L"Special (7182)",L"Special (7183)",L"Special (7192)",L"Special (7195)",L"Special (7198)",L"Special (7251)",L"Special (7254)",L"Special (7257)",L"Special (7260)",L"Special (7262)",L"Special (7263)",L"Special (7264)",L"Special (7268)",L"Special (7270)",L"Special (7271)",L"Special (7273)",L"Special (7275)",L"Special (7277)",L"Special (7284)",L"Special (7286)",L"Special (7287)",L"Special (7288)",L"Special (7289)",L"Special (7292)",L"Special (7293)",L"Special (7294)",L"Special (7295)",L"Special (7296)",L"Special (7297)",L"Special (7298)",L"Special (7299)",L"Special (7300)",L"Special (7301)",L"Special (7302)",L"Special (7304)",L"Special (7306)",L"Special (7307)",L"Special (7310)",L"Special (7320)",L"Special (7322)",L"Special (7330)",L"Special (7332)",L"Special (7333)",L"Special (7337)",L"Special (7345)",L"Special (7353)",L"Special (7367)",L"Special (7379)",L"Special (7383)",L"Special (7386)",L"Special (7390)",L"Special (7395)",L"Special (7397)",L"Special (7405)",L"Special (7411)",L"Special (7413)",L"Special (7417)",L"Special (7431)",L"Special (7432)",L"Special (7433)",L"Special (7442)",L"Special (7445)",L"Special (7448)",L"Special (7538)",L"Special (7541)",L"Special (7544)",L"Special (7545)",L"Special (7549)",L"Special (7552)",L"Special (7556)",L"Special (7558)",L"Special (7565)",L"Special (7567)",L"Special (7568)",L"Special (7569)",L"Special (7576)",L"Special (7577)",L"Special (7578)",L"Special (7581)",L"Special (7582)",L"Special (7583)",L"Special (7585)",L"Special (7587)",L"Special (7588)",L"Special (7591)",L"Special (7601)",L"Special (7603)",L"Special (7611)",L"Special (7613)",L"Special (7614)",L"Special (7618)",L"Special (7626)",L"Special (7634)",L"Special (7648)",L"Special (7660)",L"Special (7664)",L"Special (7667)",L"Special (7671)",L"Special (7676)",L"Special (7678)",L"Special (7686)",L"Special (7692)",L"Special (7694)",L"Special (7698)",L"Special (7712)",L"Special (7713)",L"Special (7714)",L"Special (7723)",L"Special (7726)",L"Special (7729)"
};
Vector<String> AppearanceGenerator::HairColorNames = {
    L"Blonde", L"Black", L"MediumBlond", L"Darkbrown", L"Platinumblonde", L"Lightbrown", L"Brown", L"Red"
};

void AppearanceGenerator::Read(Path const &filename) {
    static Set<Int> ShortHairIDs = {
        0,1,2,16,17,18,19,21,22,24,25,26,28,29,30,31,36,37,38,39,40,41,42,43,45,46,47,52,53,56,57,59,60,61,62,63,64,65,66,67,69,74,76,77,81,82,84,85,87,88,89,91,92,93,94,96,97,99,100,101,102,103,104,105,106,110,112,113,114,115,116,117,118,119,120,121,122,123,125,126,127,128,129,130,131,132,133,134,135,136,137,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,219,221,223,224,225,226,228,229,230,231,233,234,235,236,237,238,239,241,250,252,253,254,255,258,265,266,267,271,272,273,276,277,278,279,280,281,282,284,285,286,288,294,297,298,300,311,312,316,318,327,338,359,366,367,368,369,370,371,372,375,378,379,382,396,397,398,399,400,454,455,456,457,458,459,460,461,462,463,464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,480,481,482,483,485,486,488,489,490,491,492,493,494,495,496,497,498,499,500,501,502,503,504,505,506,508,509,510,511,512,513,514,515,516,518,519,520,521,539,556,558,566,598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,614,615,616,617,619,620,621,622,623,624,625,629,635,637,638,639,640,641,808
    };
    static Set<Int> MediumHairIDs = {
        3,8,10,13,14,20,23,35,44,50,51,58,68,70,72,73,80,83,86,90,95,98,107,108,109,111,124,138,167,218,222,227,232,240,242,246,247,248,249,251,256,260,263,264,268,269,270,274,275,283,287,289,290,296,299,302,313,315,320,321,322,326,328,329,333,334,335,337,339,340,341,344,345,346,350,351,353,354,356,360,361,362,363,365,373,374,376,377,380,383,385,386,387,401,404,405,406,407,426,427,432,484,487,517,540,554,557,565,584,585,587,588,589,590,591,592,593,594,613,618,626,627,628,630,643 
    };
    static Set<Int> LongHairIDs = {
        4,5,6,7,9,11,12,15,32,33,34,48,49,54,55,71,75,78,79,220,244,245,257,259,261,262,295,301,303,304,305,306,307,308,309,310,314,317,323,324,325,330,331,332,336,342,343,347,348,349,352,355,357,358,364,381,384,388,389,390,391,392,393,394,395,402,403,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,428,429,430,431,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,451,452,453,507,522,523,524,525,526,527,528,529,530,531,532,534,535,536,537,538,541,542,543,544,545,546,547,548,549,550,551,555,560,562,563,564,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,586,595,857
    };
    static Set<Int> BaldingHairstyles = {
        26,29,41,46,47,92,290,297,665
    };
    static Set<Int> BaldHairstyles = {
        0,25
    };
    FifamAppearanceDefs defs;
    defs.Read(filename);
    for (auto const &[id, def] : defs.mDefs) {
        mAppearanceDefs[id] = def;
        auto &hairsDef = mAppearanceDefsHairstyles[id];
        auto const &hairIDs = def.mParameters[FifamAppearanceDefs::ParamHair];
        for (auto const &i : hairIDs) {
            Bool bald = Utils::Contains(BaldHairstyles, i.first);
            Bool balding = Utils::Contains(BaldingHairstyles, i.first);
            if (!bald && !balding) {
                if (Utils::Contains(ShortHairIDs, i.first)) {
                    hairsDef.mHairstylesByLength[HairShort].push_back(i);
                    hairsDef.mHairstylesByLengthSum[HairShort] += i.second;
                }
                if (Utils::Contains(MediumHairIDs, i.first)) {
                    hairsDef.mHairstylesByLength[HairMedium].push_back(i);
                    hairsDef.mHairstylesByLengthSum[HairMedium] += i.second;
                }
                if (Utils::Contains(LongHairIDs, i.first)) {
                    hairsDef.mHairstylesByLength[HairLong].push_back(i);
                    hairsDef.mHairstylesByLengthSum[HairLong] += i.second;
                }
            }
            if (!balding) {
                hairsDef.mHairstylesByRule[HairWithoutBaldingWithBald].push_back(i);
                hairsDef.mHairstylesByRuleSum[HairWithoutBaldingWithBald] += i.second;
            }
            if (!bald && !balding) {
                hairsDef.mHairstylesByRule[HairWithoutBaldingAndBald].push_back(i);
                hairsDef.mHairstylesByRuleSum[HairWithoutBaldingAndBald] += i.second;
            }
        }
    }
}

Int AppearanceGenerator::GetRandomAppearanceParam(FifamAppearanceDefs::Type type, FifamAppearanceDefs::Param param, Int defaultValue) {
    auto &def = mAppearanceDefs[type];
    if (def.mParametersSum[param] > 0) {
        UInt rnd = Random::Get(0, def.mParametersSum[param] - 1);
        UInt acc = 0;
        for (UInt i = 0; i < def.mParameters[param].size(); i++) {
            acc += def.mParameters[param][i].second;
            if (rnd < acc)
                return def.mParameters[param][i].first;
        }
    }
    return defaultValue;
}

Int AppearanceGenerator::GetRandomAppearanceHairstyleByLength(FifamAppearanceDefs::Type type, AppearanceGenerator::HairLength hairLength, Int defaultValue) {
    auto &hairsDef = mAppearanceDefsHairstyles[type];
    if (hairsDef.mHairstylesByLengthSum[hairLength] > 0) {
        UInt rnd = Random::Get(0, hairsDef.mHairstylesByLengthSum[hairLength] - 1);
        UInt acc = 0;
        for (UInt i = 0; i < hairsDef.mHairstylesByLength[hairLength].size(); i++) {
            acc += hairsDef.mHairstylesByLength[hairLength][i].second;
            if (rnd < acc)
                return hairsDef.mHairstylesByLength[hairLength][i].first;
        }
    }
    return defaultValue;
}

Int AppearanceGenerator::GetRandomAppearanceHairstyleByRule(FifamAppearanceDefs::Type type, AppearanceGenerator::HairRule hairRule, Int defaultValue) {
    auto &hairsDef = mAppearanceDefsHairstyles[type];
    if (hairsDef.mHairstylesByRuleSum[hairRule] > 0) {
        UInt rnd = Random::Get(0, hairsDef.mHairstylesByRuleSum[hairRule] - 1);
        UInt acc = 0;
        for (UInt i = 0; i < hairsDef.mHairstylesByRule[hairRule].size(); i++) {
            acc += hairsDef.mHairstylesByRule[hairRule][i].second;
            if (rnd < acc)
                return hairsDef.mHairstylesByRule[hairRule][i].first;
        }
    }
    return defaultValue;
}

void AppearanceGenerator::Generate(FifamPlayer *player, FifamAppearanceDefs::Type type, UInt age) {
    UInt baldMinAge = 0;
    if (type == FifamAppearanceDefs::African1)
        baldMinAge = 28;
    else if (type == FifamAppearanceDefs::African2)
        baldMinAge = 33;
    HairRule rule = age < baldMinAge ? HairWithoutBaldingAndBald : HairWithoutBaldingWithBald;
    player->mAppearance.mSkinColor = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamSkinColor, player->mAppearance.mSkinColor);
    player->mAppearance.mGenericFace = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamFace, player->mAppearance.mGenericFace);
    player->mAppearance.mHairStyle = GetRandomAppearanceHairstyleByRule(type, rule, player->mAppearance.mHairStyle);
    player->mAppearance.mFaceVariation = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamVariation, player->mAppearance.mFaceVariation);
    player->mAppearance.mBeardColor = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamBeardColor, player->mAppearance.mBeardColor);
    player->mAppearance.mEyeColour = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamEyeColor, player->mAppearance.mEyeColour);
    player->mAppearance.mBeardType = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamBeard, player->mAppearance.mBeardType);
    player->mAppearance.mSideburns = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamSideburns, player->mAppearance.mSideburns);
    player->mAppearance.mHairColor = GetRandomAppearanceParam(type, FifamAppearanceDefs::ParamHairColor, player->mAppearance.mHairColor);
    player->mAppearance.SetBeardColorFromHairColor();
}

enum ShoeType {
    SHOE_WHITE = 1,
    SHOE_BLACK = 2,
    SHOE_BLUE = 3,
    SHOE_RED = 4,
    SHOE_GOLD = 5,
    SHOE_WHITE_BLUE = 6,
    SHOE_WHITE_BLACK = 7,
    SHOE_WHITE_LIGHT_BLUE = 8,
    SHOE_GREEN = 10,
    SHOE_BLACK_RED = 11,
    SHOE_GREY = 12,
    SHOE_WHITE_RED = 13,
    SHOE_YELLOW = 14,
    SHOE_ORANGE = 15,
    SHOE_DARK_BLUE = 16,
    SHOE_LIGHT_GREEN = 17,
    SHOE_MAGENTA = 18,
    SHOE_BRIGHT_RED = 19
};

int GetPlayerShoesIdFromFifaId(int shoeTypeCode) {
    int ShoeType = 0;
    switch (shoeTypeCode) {
    case 17:
    case 20:
    case 21:
    case 31:
    case 136:
    case 148:
    case 150:
    case 162:
    case 205:
    case 210:
    case 211:
    case 213:
    case 214:
    case 238:
    case 319:
    case 321:
    case 334:
    case 362:
    case 380:
    case 385:
        ShoeType = SHOE_WHITE;
        break;
    case 26:
    case 37:
    case 42:
    case 54:
    case 55:
    case 56:
    case 180:
    case 185:
    case 194:
    case 197:
    case 202:
    case 225:
    case 229:
    case 234:
    case 250:
    case 300:
    case 302:
    case 306:
    case 307:
    case 308:
    case 314:
    case 315:
    case 339:
    case 355:
    case 357:
    case 359:
        ShoeType = SHOE_BLACK;
        break;
    case 29:
    case 32:
    case 53:
    case 62:
    case 75:
    case 84:
    case 91:
    case 131:
    case 132:
    case 133:
    case 146:
    case 157:
    case 175:
    case 193:
    case 195:
    case 203:
    case 212:
    case 230:
    case 301:
    case 317:
    case 323:
    case 325:
    case 369:
    case 371:
        ShoeType = SHOE_BLUE;
        break;
    case 52:
    case 97:
    case 117:
    case 167:
    case 168:
    case 172:
    case 173:
    case 186:
    case 188:
    case 189:
    case 249:
    case 340:
        ShoeType = SHOE_RED;
        break;
    case 28:
    case 35:
    case 66:
    case 67:
    case 68:
    case 71:
    case 98:
    case 253:
        ShoeType = SHOE_GOLD;
        break;
    case 18:
    case 108:
    case 247:
    case 311:
    case 316:
    case 363:
    case 378:
    case 379:
        ShoeType = SHOE_WHITE_BLUE;
        break;
    case 60:
    case 70:
    case 100:
    case 101:
    case 104:
    case 128:
    case 134:
    case 153:
    case 154:
    case 170:
    case 171:
    case 303:
    case 304:
    case 338:
    case 360:
    case 389:
    case 441:
    case 442:
        ShoeType = SHOE_WHITE_BLACK;
        break;
    case 72:
    case 74:
    case 92:
    case 94:
    case 159:
    case 222:
    case 305:
    case 318:
    case 368:
        ShoeType = SHOE_WHITE_LIGHT_BLUE;
        break;
    case 83:
    case 88:
    case 109:
    case 143:
    case 152:
    case 165:
    case 166:
    case 176:
    case 177:
    case 219:
    case 223:
    case 224:
    case 226:
    case 243:
    case 244:
    case 246:
    case 332:
    case 431:
    case 432:
        ShoeType = SHOE_GREEN;
        break;
    case 41:
    case 57:
    case 58:
    case 59:
    case 77:
    case 95:
    case 115:
    case 130:
    case 137:
    case 142:
    case 155:
    case 169:
    case 179:
    case 184:
    case 227:
    case 228:
    case 233:
    case 326:
    case 384:
        ShoeType = SHOE_BLACK_RED;
        break;
    case 22:
    case 27:
    case 30:
    case 33:
    case 39:
    case 47:
    case 61:
    case 80:
    case 82:
    case 107:
    case 149:
    case 160:
    case 191:
    case 192:
    case 196:
    case 198:
    case 204:
    case 207:
    case 236:
    case 237:
    case 312:
    case 320:
    case 324:
    case 330:
    case 333:
    case 354:
    case 356:
    case 358:
        ShoeType = SHOE_GREY;
        break;
    case 43:
    case 45:
    case 46:
    case 50:
    case 69:
    case 85:
    case 96:
    case 102:
    case 112:
    case 140:
    case 141:
    case 145:
    case 147:
    case 156:
    case 220:
    case 241:
    case 242:
    case 245:
    case 309:
    case 313:
    case 322:
    case 383:
        ShoeType = SHOE_WHITE_RED;
        break;
    case 81:
    case 86:
    case 99:
    case 110:
    case 113:
    case 121:
    case 122:
    case 164:
    case 182:
        ShoeType = SHOE_YELLOW;
        break;
    case 19:
    case 36:
    case 51:
    case 78:
    case 79:
    case 116:
    case 129:
    case 139:
    case 144:
    case 174:
    case 181:
    case 183:
    case 187:
    case 199:
    case 200:
    case 201:
    case 252:
    case 254:
    case 310:
    case 327:
    case 328:
    case 331:
    case 381:
    case 382:
        ShoeType = SHOE_ORANGE;
        break;
    case 38:
    case 40:
    case 64:
    case 73:
    case 93:
    case 135:
    case 161:
    case 190:
    case 206:
    case 221:
    case 231:
    case 235:
        ShoeType = SHOE_DARK_BLUE;
        break;
    case 16:
    case 23:
    case 25:
    case 34:
    case 63:
    case 76:
    case 87:
    case 90:
    case 105:
    case 106:
    case 111:
    case 114:
    case 125:
    case 126:
    case 138:
    case 158:
    case 163:
    case 215:
    case 216:
    case 217:
    case 218:
    case 232:
    case 335:
        ShoeType = SHOE_LIGHT_GREEN;
        break;
    case 48:
    case 49:
    case 65:
    case 89:
    case 118:
    case 119:
    case 120:
    case 123:
    case 124:
    case 127:
    case 178:
    case 239:
    case 240:
    case 248:
    case 251:
    case 329:
    case 336:
    case 337:
    case 364:
    case 365:
    case 366:
    case 367:
    case 370:
        ShoeType = SHOE_MAGENTA;
        break;
    case 24:
    case 44:
    case 103:
    case 151:
    case 208:
    case 209:
    case 361:
        ShoeType = SHOE_BRIGHT_RED;
        break;
    default:
        ShoeType = Random::Get(1, 19);
    }
    return ShoeType;
}

void AppearanceGenerator::SetFromFifaPlayer(FifamPlayer *player, FifaPlayer *fifaPlayer) {
    bool isLastVersion = fifaPlayer->m_gameId == FifaDatabase::m_lastSupportedGameVersion;
    switch (fifaPlayer->internal.skintonecode) {
    case 1:
        player->mAppearance.mSkinColor = 0;
        break;
    case 2:
        player->mAppearance.mSkinColor = 1;
        break;
    case 3:
        player->mAppearance.mSkinColor = 2;
        break;
    case 4:
        player->mAppearance.mSkinColor = 3;
        break;
    case 5:
        player->mAppearance.mSkinColor = 4;
        break;
    case 6:
        player->mAppearance.mSkinColor = 5;
        break;
    case 7:
        player->mAppearance.mSkinColor = 6;
        break;
    case 8:
        player->mAppearance.mSkinColor = 7;
        break;
    case 9:
        player->mAppearance.mSkinColor = 8;
        break;
    case 10:
        player->mAppearance.mSkinColor = 9;
        break;
    }
    // FM 26 mFaceVariation: 0 - normal, 1 - freckles, 2 - wrinkles, 3 - female normal, 4 - female freckles, 5 - female wrinkles,
    //                       6 - female makeup 1, 7 - female makeup 2
    if (fifaPlayer->m_gameId >= 25) {
        // FC 25 skintypecode: 0 - normal, 1-5 - freckles, 6 - five o'clock shadow, 7 - aging spots
        // FC 25 skinsurfacepack
        // FC 25 skinmakeup: = 0 - none, 1-3 - makeup types
        if (fifaPlayer->IsMale()) {
            player->mAppearance.mFaceVariation = 0; // normal
            if (fifaPlayer->internal.skintypecode >= 1 && fifaPlayer->internal.skintypecode <= 5)
                player->mAppearance.mFaceVariation = 1; // freckles
            else {
                switch (fifaPlayer->internal.skinsurfacepack) {
                case 232201: // Male 30s Caucasian Heavy 01
                case 240201: // Male 40s African Heavy 01
                case 240202: // Male 40s African Heavy 02
                case 242201: // Male 40s Caucasian Heavy 01
                case 250201: // Male 50s African Heavy 01
                case 250202: // Male 50s African Heavy 02
                case 252001: // Male 50s Caucasian Thin 01
                    player->mAppearance.mFaceVariation = 2; // wrinkles
                    break;
                }
            }
        }
        else {
            player->mAppearance.mFaceVariation = 3; // female normal
            if (fifaPlayer->internal.skintypecode >= 1 && fifaPlayer->internal.skintypecode <= 5)
                player->mAppearance.mFaceVariation = 4; // female freckles
            else {
                if (fifaPlayer->internal.skinmakeup != 0) {
                    if (fifaPlayer->internal.skinmakeup == 1)
                        player->mAppearance.mFaceVariation = 6; // female makeup 1
                    else
                        player->mAppearance.mFaceVariation = 7; // female makeup 2
                }
                else {
                    switch (fifaPlayer->internal.skinsurfacepack) {
                    case 122201: // Female 20s Caucasian Heavy 01
                    case 131201: // Female 30s Asian Heavy 01
                    case 132201: // Female 30s Caucasian Heavy 01
                    case 132202: // Female 30s Caucasian Heavy 02
                    case 141001: // Female 40s Asian Thin 01
                        player->mAppearance.mFaceVariation = 5; // female wrinkles
                        break;
                    }
                }
            }
        }
    }
    else {
        // skintypecode: 0 - normal, 1 - freckles, 2 - wrinkles, 5-7 - ?, 50 - young, 51 - old,
        //              100 - female normal, 101 - female wrinkles, 102 - female makeup 1, 103 - female makeup 2,
        //              104 - female freckles, 150 - female young, 151 - female old
        if (fifaPlayer->IsMale()) {
            player->mAppearance.mFaceVariation = 0; // normal
            switch (fifaPlayer->internal.skintypecode) {
            case 1:
            case 104:
                player->mAppearance.mFaceVariation = 1; // freckles
                break;
            case 2:
            case 51:
            case 101:
            case 151:
                player->mAppearance.mFaceVariation = 2; // wrinkles
                break;
            }
        }
        else {
            player->mAppearance.mFaceVariation = 3; // female normal
            switch (fifaPlayer->internal.skintypecode) {
            case 1:
            case 104:
                player->mAppearance.mFaceVariation = 4; // female freckles
                break;
            case 2:
            case 51:
            case 101:
            case 151:
                player->mAppearance.mFaceVariation = 5; // female wrinkles
                break;
            case 102:
                player->mAppearance.mFaceVariation = 6; // female makeup 1
                break;
            case 103:
                player->mAppearance.mFaceVariation = 7; // female makeup 2
                break;
            }
        }
    }
    // 0 - light blue 1 - brown 2 - grey-green 3 - green 4 - green-blue 5 -grey 6 - blue
    switch (fifaPlayer->internal.eyecolorcode) {
    case 1:
        player->mAppearance.mEyeColour = 6;
        break;
    case 2:
        player->mAppearance.mEyeColour = 0;
        break;
    case 3:
        player->mAppearance.mEyeColour = 1;
        break;
    case 4:
        player->mAppearance.mEyeColour = 7;
        break;
    case 5:
        player->mAppearance.mEyeColour = 4;
        break;
    case 6:
        player->mAppearance.mEyeColour = 3;
        break;
    case 7:
        player->mAppearance.mEyeColour = 9;
        break;
    case 8:
        player->mAppearance.mEyeColour = 5;
        break;
    case 9:
        player->mAppearance.mEyeColour = 8;
        break;
    case 10:
        player->mAppearance.mEyeColour = 2;
        break;
    }
    if (fifaPlayer->internal.sideburnscode)
        player->mAppearance.mSideburns = 1;
    else
        player->mAppearance.mSideburns = 0;
    if (fifaPlayer->IsMale()) {
        switch (fifaPlayer->internal.facialhairtypecode) {
        case 0:
        case 246:
            player->mAppearance.mBeardType = 0; // Clean Shaved
            break;
        case 1:
        case 254:
        case 259:
        case 288:
            player->mAppearance.mBeardType = 1; // Chin Stubble Light  _
            break;
        case 2:
        case 285:
            player->mAppearance.mBeardType = 2; // Chin Strap \_/
            break;
        case 3:
        case 251:
            player->mAppearance.mBeardType = 3; // Goatee |_|
            break;
        case 4:
        case 28:
        case 38:
        case 45:
        case 50:
        case 53:
        case 64:
        case 243:
        case 255:
            player->mAppearance.mBeardType = 4; // Casual Beard \|_|/
            break;
        case 5:
        case 27:
        case 31:
        case 32:
            player->mAppearance.mBeardType = 5; // PartialGoatee |_|
            break;
        case 6:
        case 57:
            player->mAppearance.mBeardType = 6; // Stubble \|_|/
            break;
        case 7:
            player->mAppearance.mBeardType = 7; // Tuft \/
            break;
        case 8:
        case 18:
        case 20:
        case 21:
        case 25:
        case 30:
        case 34:
        case 35:
        case 39:
        case 41:
        case 54:
        case 61:
        case 62:
        case 63:
        case 65:
        case 66:
        case 67:
        case 238:
        case 245:
        case 249:
        case 252:
        case 266:
        case 270:
        case 289:
            player->mAppearance.mBeardType = 8; // Full Beard \|_|/
            break;
        case 9:
        case 16:
        case 17:
        case 29:
        case 46:
        case 47:
        case 48:
        case 49:
        case 257:
        case 286:
            player->mAppearance.mBeardType = 9; // Light Goatee |_|
            break;
        case 10:
        case 33:
        case 36:
        case 42:
        case 43:
            player->mAppearance.mBeardType = 10; // Moustache --
            break;
        case 11:
        case 236:
            player->mAppearance.mBeardType = 11; // Light Chin Curtain \\//
            break;
        case 12:
        case 19:
        case 60:
        case 248:
        case 253:
        case 258:
            player->mAppearance.mBeardType = 12; // FullGoatee |_|
            break;
        case 13:
        case 22:
        case 237:
        case 247:
        case 260:
        case 272:
            player->mAppearance.mBeardType = 13; // Chin Curtain \\//
            break;
        case 14:
        case 23:
        case 26:
        case 44:
        case 51:
        case 55:
        case 56:
        case 58:
        case 244:
            player->mAppearance.mBeardType = 14; // Beard \|_|/
            break;
        case 15:
        case 24:
        case 37:
        case 40:
        case 52:
        case 59:
        case 287:
            player->mAppearance.mBeardType = 15; // Patchy Beard \|_|/
            break;
        }
    }
    else
        player->mAppearance.mBeardType = 0; // Clean Shaved
    // 0 - black, 1 - blonde, 2 - brown, 3 - medium blonde, 4 - red
    if (fifaPlayer->m_gameId >= 23) {
        switch (fifaPlayer->internal.facialhaircolorcode) {
        case 0:
        case 8:
        case 9:
        case 10:
        case 11:
        case 21:
        case 22:
        case 23:
        case 24:
        case 32:
        case 33:
        case 34:
            player->mAppearance.mBeardColor = 0; // black
            break;
        case 4:
        case 15:
        case 16:
        case 17:
        case 18:
            player->mAppearance.mBeardColor = 1; // blonde
            break;
        case 3:
        case 6:
        case 12:
        case 13:
            player->mAppearance.mBeardColor = 2; // brown
            break;
        case 1:
        case 2:
        case 5:
        case 19:
        case 20:
            player->mAppearance.mBeardColor = 3; // medium blonde (light brown)
            break;
        case 7:
        case 14:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            player->mAppearance.mBeardColor = 4; // red
            break;
        }
    }
    else {
        switch (fifaPlayer->internal.facialhaircolorcode) {
        case 0:
            player->mAppearance.mBeardColor = 0; // black
            break;
        case 1:
            player->mAppearance.mBeardColor = 1; // blonde
            break;
        case 2:
            player->mAppearance.mBeardColor = 2; // brown
            break;
        case 3:
            player->mAppearance.mBeardColor = 3; // medium blonde (light brown)
            break;
        case 4:
            player->mAppearance.mBeardColor = 4; // red
            break;
        }
    }

    static Map<UInt, UInt> fifaHairMapping = {
        // direct mapping
        {0,0},{1,1},{2,2},{3,3},{4,4},{5,5},{6,6},{7,7},{8,8},{9,9},{10,10},{11,11},{12,12},{13,13},{14,14},{15,15},{16,16},{17,17},{18,18},{19,19},{20,20},{21,21},{22,22},{23,23},{24,24},{25,25},{26,26},{27,27},{28,28},{29,29},{30,30},{31,31},{32,32},{33,33},{34,34},{35,35},{36,36},{37,37},{38,38},{39,39},{40,40},{41,41},{42,42},{43,43},{44,44},{45,45},{46,46},{47,47},{48,48},{49,49},{51,50},{52,51},{53,52},{54,53},{55,54},{56,55},{57,56},{58,57},{59,58},{60,59},{61,60},{62,61},{63,62},{64,63},{65,64},{66,65},{67,66},{68,67},{69,68},{70,69},{71,70},{72,71},{73,72},{74,73},{75,74},{76,75},{77,76},{78,77},{79,78},{80,79},{81,80},{82,81},{83,82},{84,83},{85,84},{86,85},{87,86},{88,87},{89,88},{90,89},{91,90},{92,91},{93,92},{94,93},{95,94},{96,95},{97,96},{98,97},{99,98},{100,99},{101,100},{102,101},{103,102},{104,103},{105,104},{106,105},{107,106},{108,107},{109,108},{110,109},{111,110},{112,111},{113,112},{114,113},{115,114},{116,115},{117,116},{118,117},{119,118},{120,119},{121,120},{122,121},{123,122},{124,123},{125,124},{126,125},{127,126},{128,127},{129,128},{130,129},{131,130},{132,131},{133,132},{134,133},{135,134},{136,135},{137,136},{138,137},{139,138},{140,139},{141,140},{142,141},{143,142},{144,143},{145,144},{146,145},{147,146},{148,147},{149,148},{150,149},{151,150},{152,151},{153,152},{154,153},{155,154},{156,155},{157,156},{158,157},{159,158},{160,159},{161,160},{162,161},{163,162},{164,163},{165,164},{166,165},{167,166},{168,167},{169,168},{170,169},{171,170},{172,171},{173,172},{174,173},{175,174},{176,175},{177,176},{178,177},{179,178},{180,179},{181,180},{182,181},{183,182},{184,183},{185,184},{186,185},{187,186},{188,187},{189,188},{190,189},{191,190},{192,191},{193,192},{194,193},{195,194},{196,195},{197,196},{198,197},{199,198},{200,199},{201,200},{202,201},{203,202},{204,203},{205,204},{206,205},{207,206},{208,207},{209,208},{210,209},{211,210},{212,211},{218,212},{219,213},{220,214},{221,215},{222,216},{223,217},{224,218},{225,219},{226,220},{227,221},{228,222},{229,223},{230,224},{231,225},{232,226},{233,227},{234,228},{235,229},{236,230},{237,231},{238,232},{239,233},{240,234},{241,235},{242,236},{243,237},{244,238},{245,239},{246,240},{250,241},{251,242},{253,243},{255,244},{256,245},{257,246},{258,247},{260,248},{261,249},{262,250},{263,251},{264,252},{265,253},{266,254},{267,255},{268,256},{269,257},{270,258},{271,259},{272,260},{273,261},{274,262},{275,263},{276,264},{277,265},{278,266},{279,267},{280,268},{281,269},{282,270},{283,271},{284,272},{285,273},{286,274},{288,275},{289,276},{290,277},{291,278},{292,279},{293,280},{294,281},{295,282},{296,283},{297,284},{298,285},{299,286},{374,287},{375,288},{376,289},{381,290},{385,291},{386,292},{389,293},{390,294},{392,295},{394,296},{398,297},{399,298},{418,299},{419,300},{421,301},{431,302},{432,303},{433,304},{434,305},{435,306},{436,307},{437,308},{438,309},{444,310},{445,311},{446,312},{447,313},{448,314},{450,315},{451,316},{452,317},{460,318},{461,319},{462,320},{463,321},{492,322},{494,323},{500,324},{501,325},{502,326},{503,327},{504,328},{505,329},{506,330},{507,331},{508,332},{509,333},{510,334},{511,335},{512,336},{513,337},{514,338},{515,339},{516,340},{517,341},{518,342},{519,343},{520,344},{521,345},{522,346},{523,347},{524,348},{525,349},{526,350},{527,351},{528,352},{529,353},{530,354},{531,355},{532,356},{533,357},{534,358},{535,359},{536,360},{537,361},{538,362},{539,363},{540,364},{541,365},{542,366},{543,367},{544,368},{545,369},{546,370},{547,371},{548,372},{549,373},{550,374},{551,375},{552,376},{553,377},{554,378},{555,379},{556,380},{557,381},{558,382},{559,383},{560,384},{561,385},{562,386},{563,387},{564,388},{565,389},{566,390},{567,391},{568,392},{569,393},{570,394},{571,395},{572,396},{573,397},{574,398},{575,399},{576,400},{577,401},{578,402},{579,403},{580,404},{581,405},{582,406},{583,407},{584,408},{585,409},{586,410},{587,411},{588,412},{589,413},{590,414},{591,415},{592,416},{593,417},{594,418},{595,419},{596,420},{597,421},{598,422},{599,423},{600,424},{601,425},{602,426},{603,427},{604,428},{605,429},{606,430},{607,431},{608,432},{609,433},{610,434},{611,435},{612,436},{613,437},{614,438},{615,439},{616,440},{617,441},{618,442},{619,443},{620,444},{621,445},{622,446},{623,447},{624,448},{625,449},{626,450},{627,451},{628,452},{629,453},{630,454},{631,455},{632,456},{633,457},{634,458},{635,459},{636,460},{637,461},{638,462},{639,463},{640,464},{641,465},{642,466},{643,467},{644,468},{645,469},{646,470},{647,471},{648,472},{649,473},{650,474},{651,475},{652,476},{653,477},{654,478},{655,479},{656,480},{657,481},{659,482},{660,483},{661,484},{662,485},{663,486},{664,487},{665,488},{666,489},{667,490},{668,491},{669,492},{670,493},{671,494},{672,495},{673,496},{674,497},{675,498},{676,499},{677,500},{678,501},{679,502},{680,503},{681,504},{682,505},{683,506},{684,507},{685,508},{686,509},{687,510},{688,511},{689,512},{690,513},{691,514},{692,515},{693,516},{694,517},{695,518},{696,519},{697,520},{698,521},{720,522},{721,523},{722,524},{723,525},{724,526},{725,527},{726,528},{727,529},{728,530},{729,531},{730,532},{731,533},{732,534},{733,535},{734,536},{735,537},{736,538},{737,539},{738,540},{739,541},{740,542},{741,543},{742,544},{743,545},{744,546},{745,547},{746,548},{747,549},{748,550},{749,551},{810,552},{811,553},{815,554},{816,555},{828,556},{831,557},{832,558},{845,559},{881,560},{890,561},{891,562},{892,563},{900,564},{901,565},{902,566},{903,567},{904,568},{905,569},{906,570},{907,571},{908,572},{909,573},{910,574},{911,575},{912,576},{913,577},{914,578},{915,579},{916,580},{917,581},{918,582},{919,583},{920,584},{921,585},{922,586},{923,587},{924,588},{925,589},{926,590},{927,591},{928,592},{930,593},{931,594},{932,595},{1059,596},{1060,597},{1061,598},{1062,599},{1063,600},{1064,601},{1065,602},{1066,603},{1067,604},{1068,605},{1069,606},{1070,607},{1071,608},{1072,609},{1073,610},{1074,611},{1075,612},{1076,613},{1077,614},{1078,615},{1079,616},{1080,617},{1081,618},{1082,619},{1083,620},{1084,621},{1085,622},{1086,623},{1087,624},{1088,625},{1089,626},{1090,627},{1091,628},{1092,629},{1093,630},{1094,631},{1095,632},{1096,633},{1099,634},{1106,635},{1157,636},{1281,637},{1282,638},{1283,639},{1284,640},{1285,641},{1316,642},{1317,643},{1337,644},{1338,645},{1341,646},{1368,647},{1370,648},{1373,649},{1374,650},{1377,651},{1378,652},{1384,653},{1385,654},{1386,655},{2000,656},{2001,657},{2002,658},{2003,659},{2004,660},{2005,661},{2006,662},{2007,663},{2008,664},{2009,665},{2010,666},{2011,667},{2012,668},{2013,669},{2014,670},{2015,671},{2016,672},{2017,673},{2018,674},{2019,675},{2020,676},{2021,677},{2022,678},{2023,679},{2024,680},{2025,681},{2026,682},{2027,683},{2028,684},{2029,685},{2030,686},{2031,687},{2032,688},{2033,689},{2034,690},{2035,691},{2036,692},{2037,693},{2038,694},{2039,695},{2040,696},{2041,697},{2042,698},{2043,699},{2044,700},{2045,701},{2046,702},{2047,703},{2048,704},{2049,705},{2050,706},{2051,707},{2052,708},{2053,709},{2054,710},{2055,711},{2056,712},{2057,713},{2058,714},{2059,715},{2060,716},{2061,717},{2062,718},{2063,719},{2064,720},{2065,721},{2066,722},{2067,723},{2068,724},{3000,725},{3001,726},{3002,727},{3003,728},{3004,729},{3005,730},{3006,731},{3007,732},{3008,733},{3009,734},{3010,735},{3011,736},{3012,737},{3013,738},{3014,739},{3015,740},{3016,741},{3017,742},{3018,743},{3019,744},{3020,745},{3021,746},{3022,747},{3025,748},{3026,749},{3027,750},{3028,751},{3029,752},{3030,753},{3031,754},{3032,755},{3033,756},{3034,757},{3035,758},{3036,759},{3037,760},{3038,761},{3039,762},{3040,763},{3041,764},{3042,765},{3043,766},{3044,767},{3045,768},{3046,769},{3047,770},{3048,771},{3049,772},{3050,773},{3051,774},{3052,775},{3053,776},{3055,777},{3056,778},{3057,779},{3058,780},{3059,781},{3060,782},{3061,783},{3062,784},{3063,785},{3064,786},{3065,787},{3066,788},{3067,789},{3068,790},{3069,791},{3070,792},{3071,793},{3072,794},{3073,795},{3074,796},{3075,797},{3076,798},{3077,799},{3078,800},{3079,801},{3080,802},{3081,803},{3082,804},{3083,805},{3084,806},{3085,807},{3086,808},{3087,809},{3088,810},{3089,811},{3090,812},{3091,813},{3092,814},{3093,815},{3094,816},{3095,817},{3096,818},{3097,819},{3098,820},{3100,821},{3101,822},{3102,823},{3103,824},{3104,825},{3105,826},{3106,827},{3107,828},{3108,829},{3109,830},{3110,831},{3111,832},{3112,833},{3113,834},{3114,835},{3115,836},{3116,837},{3117,838},{3118,839},{3119,840},{3120,841},{3121,842},{3122,843},{3123,844},{3124,845},{3125,846},{3126,847},{3127,848},{3128,849},{3129,850},{3130,851},{3131,852},{3132,853},{3133,854},{3134,855},{3135,856},{3136,857},{3137,858},{3138,859},{3139,860},{3140,861},{3141,862},{3142,863},{3143,864},{3144,865},{3145,866},{3146,867},{3147,868},{3148,869},{3149,870},{3150,871},{3151,872},{3152,873},{3153,874},{3154,875},{3155,876},{3156,877},{3157,878},{3158,879},{3159,880},{3160,881},{3161,882},{3162,883},{3163,884},{3164,885},{3165,886},{3166,887},{3167,888},{3168,889},{3169,890},{3170,891},{3171,892},{3172,893},{3173,894},{3174,895},{3175,896},{3176,897},{3177,898},{3178,899},{3179,900},{3180,901},{3181,902},{3182,903},{3183,904},{3184,905},{3185,906},{3186,907},{3187,908},{3188,909},{3189,910},{3190,911},{3191,912},{3192,913},{3193,914},{3194,915},{3195,916},{3196,917},{3197,918},{3198,919},{3199,920},{3200,921},{3201,922},{3202,923},{3203,924},{3204,925},{3205,926},{3206,927},{3207,928},{3208,929},{3209,930},{3210,931},{3211,932},{3212,933},{3213,934},{3214,935},{3215,936},{3216,937},{3217,938},{3218,939},{3219,940},{3220,941},{3221,942},{3232,943},{3233,944},{3234,945},{3237,946},{3238,947},{3239,948},{3240,949},{3241,950},{3242,951},{3244,952},{3245,953},{3246,954},{3247,955},{3248,956},{3249,957},{3250,958},{3251,959},{3252,960},{3253,961},{3254,962},{3255,963},{3256,964},{3257,965},{3258,966},{3259,967},{3260,968},{3261,969},{3262,970},{3263,971},{3264,972},{3265,973},{3266,974},{3267,975},{3268,976},{3269,977},{3270,978},{3271,979},{3272,980},{3273,981},{3274,982},{3275,983},{3276,984},{3277,985},{3278,986},{3279,987},{3280,988},{3281,989},{3282,990},{3283,991},{3284,992},{3285,993},{3298,994},{3300,995},{4000,996},{4001,997},{4002,998},{4003,999},{4004,1000},{4005,1001},{4006,1002},{4007,1003},{4008,1004},{4009,1005},{4010,1006},{4011,1007},{4012,1008},{4013,1009},{4014,1010},{4015,1011},{4016,1012},{4017,1013},{4018,1014},{4019,1015},{4020,1016},{4021,1017},{4022,1018},{4023,1019},{4024,1020},{4025,1021},{4026,1022},{4027,1023},{4028,1024},{4029,1025},{4030,1026},{5000,1027},{5001,1028},{5002,1029},{5003,1030},{5004,1031},{5005,1032},{5006,1033},{5007,1034},{5008,1035},{5009,1036},{5010,1037},{5011,1038},{5012,1039},{5013,1040},{5014,1041},{5015,1042},{5016,1043},{5017,1044},{6000,1045},{6001,1046},{6002,1047},{6003,1048},{6004,1049},{6005,1050},{6006,1051},{6007,1052},{6008,1053},{6009,1054},{6010,1055},{6011,1056},{6012,1057},{6013,1058},{6014,1059},{6015,1060},{6016,1061},{6017,1062},{6018,1063},{6019,1064},{6020,1065},{6021,1066},{6022,1067},{6023,1068},{6024,1069},{6025,1070},{6026,1071},{6033,1072},{6035,1073},{6036,1074},{6037,1075},{6038,1076},{6039,1077},{6040,1078},{6041,1079},{6042,1080},{7007,1081},{7010,1082},{7013,1083},{7014,1084},{7018,1085},{7021,1086},{7025,1087},{7027,1088},{7034,1089},{7036,1090},{7037,1091},{7038,1092},{7045,1093},{7046,1094},{7047,1095},{7050,1096},{7051,1097},{7052,1098},{7054,1099},{7056,1100},{7057,1101},{7060,1102},{7070,1103},{7072,1104},{7080,1105},{7082,1106},{7083,1107},{7087,1108},{7095,1109},{7103,1110},{7117,1111},{7129,1112},{7136,1113},{7140,1114},{7145,1115},{7147,1116},{7155,1117},{7161,1118},{7163,1119},{7167,1120},{7181,1121},{7182,1122},{7183,1123},{7192,1124},{7195,1125},{7198,1126},{7251,1127},{7254,1128},{7257,1129},{7260,1130},{7262,1131},{7263,1132},{7264,1133},{7268,1134},{7270,1135},{7271,1136},{7273,1137},{7275,1138},{7277,1139},{7284,1140},{7286,1141},{7287,1142},{7288,1143},{7289,1144},{7292,1145},{7293,1146},{7294,1147},{7295,1148},{7296,1149},{7297,1150},{7298,1151},{7299,1152},{7300,1153},{7301,1154},{7302,1155},{7304,1156},{7306,1157},{7307,1158},{7310,1159},{7320,1160},{7322,1161},{7330,1162},{7332,1163},{7333,1164},{7337,1165},{7345,1166},{7353,1167},{7367,1168},{7379,1169},{7383,1170},{7386,1171},{7390,1172},{7395,1173},{7397,1174},{7405,1175},{7411,1176},{7413,1177},{7417,1178},{7431,1179},{7432,1180},{7433,1181},{7442,1182},{7445,1183},{7448,1184},{7538,1185},{7541,1186},{7544,1187},{7545,1188},{7549,1189},{7552,1190},{7556,1191},{7558,1192},{7565,1193},{7567,1194},{7568,1195},{7569,1196},{7576,1197},{7577,1198},{7578,1199},{7581,1200},{7582,1201},{7583,1202},{7585,1203},{7587,1204},{7588,1205},{7591,1206},{7601,1207},{7603,1208},{7611,1209},{7613,1210},{7614,1211},{7618,1212},{7626,1213},{7634,1214},{7648,1215},{7660,1216},{7664,1217},{7667,1218},{7671,1219},{7676,1220},{7678,1221},{7686,1222},{7692,1223},{7694,1224},{7698,1225},{7712,1226},{7713,1227},{7714,1228},{7723,1229},{7726,1230},{7729,1231}
    };
    if (Utils::Contains(fifaHairMapping, (UInt)fifaPlayer->internal.hairtypecode))
        player->mAppearance.mHairStyle = fifaHairMapping[fifaPlayer->internal.hairtypecode];

    // 0 - blonde, 1 - black, 2 - medium blonde, 3 - darkbrown, 4 - platinumblonde, 5 - lightbrown, 6 - brown, 7 - red
    if (fifaPlayer->m_gameId >= 23) {
        switch (fifaPlayer->internal.haircolorcode) {
        case 17:
        case 18:
            player->mAppearance.mHairColor = 0; // blonde
            break;
        case 0:
        case 10:
        case 11:
        case 21:
        case 22:
        case 32:
        case 33:
        case 34:
            player->mAppearance.mHairColor = 1; // black
            break;
        case 4:
        case 16:
            player->mAppearance.mHairColor = 2; // medium blonde
            break;
        case 3:
        case 13:
            player->mAppearance.mHairColor = 3; // dark brown
            break;
        case 15:
            player->mAppearance.mHairColor = 4; // platinum blonde
            break;
        case 1:
        case 2:
        case 5:
        case 19:
        case 20:
            player->mAppearance.mHairColor = 5; // light brown
            break;
        case 6:
        case 12:
            player->mAppearance.mHairColor = 6; // brown
            break;
        case 7:
        case 14:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
            player->mAppearance.mHairColor = 7; // red
            break;
        case 8:
        case 9:
        case 23:
        case 24:
            player->mAppearance.mHairColor = 8; // gray
            break;
        }
    }
    else {
        switch (fifaPlayer->internal.haircolorcode) {
        case 0:
            player->mAppearance.mHairColor = 0; // blonde
            break;
        case 1:
            player->mAppearance.mHairColor = 1; // black
            break;
        case 2:
        case 9:
            player->mAppearance.mHairColor = 2; // medium blonde
            break;
        case 3:
            player->mAppearance.mHairColor = 3; // dark brown
            break;
        case 4:
            player->mAppearance.mHairColor = 4; // platinum blonde
            break;
        case 5:
            player->mAppearance.mHairColor = 5; // light brown
            break;
        case 6:
            player->mAppearance.mHairColor = 6; // brown
            break;
        case 7:
        case 12:
        case 13:
            player->mAppearance.mHairColor = 7; // red
            break;
        case 8:
            player->mAppearance.mHairColor = 8; // gray
            break;
        }
    }

    static Map<UInt, UInt> fifaHeadMapping = {
        // direct mapping
        {0,0},{1,1},{2,2},{3,3},{4,4},{5,5},{6,6},{7,7},{8,8},{9,9},{10,10},{11,11},{12,12},{13,13},{14,14},{15,15},{16,16},{17,17},{18,18},{19,19},{20,20},{21,21},{22,22},{23,23},{24,24},{25,25},{39,26},{40,27},{41,28},{42,29},{43,30},{44,31},{46,32},{47,33},{48,34},{49,35},{50,36},{51,37},{52,38},{53,39},{54,40},{55,41},{56,42},{57,43},{58,44},{59,45},{60,46},{61,47},{62,48},{63,49},{64,50},{65,51},{66,52},{67,53},{68,54},{69,55},{500,56},{501,57},{502,58},{503,59},{504,60},{505,61},{506,62},{507,63},{508,64},{509,65},{510,66},{511,67},{512,68},{513,69},{514,70},{515,71},{516,72},{517,73},{518,74},{519,75},{520,76},{521,77},{522,78},{523,79},{524,80},{525,81},{526,82},{527,83},{528,84},{529,85},{530,86},{531,87},{532,88},{533,89},{534,90},{535,91},{536,92},{537,93},{538,94},{539,95},{540,96},{541,97},{542,98},{543,99},{544,100},{545,101},{546,102},{547,103},{548,104},{549,105},{550,106},{551,107},{552,108},{553,109},{554,110},{555,111},{556,112},{557,113},{558,114},{559,115},{560,116},{561,117},{562,118},{563,119},{565,120},{567,121},{568,122},{600,123},{601,124},{602,125},{603,126},{604,127},{605,128},{606,129},{607,130},{608,131},{1000,132},{1001,133},{1002,134},{1003,135},{1004,136},{1005,137},{1006,138},{1007,139},{1008,140},{1009,141},{1010,142},{1011,143},{1012,144},{1013,145},{1014,146},{1015,147},{1016,148},{1017,149},{1018,150},{1019,151},{1020,152},{1021,153},{1022,154},{1023,155},{1024,156},{1025,157},{1026,158},{1027,159},{1030,160},{1031,161},{1033,162},{1035,163},{1036,164},{1037,165},{1038,166},{1039,167},{1040,168},{1041,169},{1042,170},{1043,171},{1044,172},{1045,173},{1046,174},{1047,175},{1048,176},{1049,177},{1050,178},{1051,179},{1052,180},{1500,181},{1501,182},{1502,183},{1503,184},{1504,185},{1505,186},{1506,187},{1507,188},{1508,189},{1509,190},{1510,191},{1511,192},{1512,193},{1513,194},{1514,195},{1515,196},{1516,197},{1517,198},{1518,199},{1519,200},{1520,201},{1521,202},{1522,203},{1523,204},{1524,205},{1525,206},{1526,207},{1527,208},{1528,209},{1530,210},{1531,211},{1532,212},{1533,213},{1534,214},{1535,215},{1536,216},{1540,217},{1541,218},{1542,219},{1543,220},{1545,221},{1546,222},{1547,223},{1548,224},{1549,225},{1550,226},{1551,227},{1552,228},{1553,229},{1554,230},{1555,231},{1556,232},{2000,233},{2001,234},{2002,235},{2003,236},{2004,237},{2005,238},{2006,239},{2007,240},{2008,241},{2009,242},{2010,243},{2011,244},{2012,245},{2013,246},{2014,247},{2015,248},{2016,249},{2017,250},{2019,251},{2020,252},{2021,253},{2022,254},{2023,255},{2024,256},{2025,257},{2026,258},{2027,259},{2028,260},{2029,261},{2032,262},{2500,263},{2501,264},{2502,265},{2503,266},{2504,267},{2505,268},{2506,269},{2507,270},{2508,271},{2509,272},{2510,273},{2511,274},{2512,275},{2513,276},{2514,277},{2515,278},{2516,279},{2517,280},{2518,281},{2520,282},{2521,283},{2522,284},{3000,285},{3001,286},{3002,287},{3003,288},{3004,289},{3005,290},{3500,291},{3501,292},{3502,293},{3503,294},{3504,295},{3505,296},{3507,297},{3509,298},{3510,299},{3512,300},{3513,301},{3514,302},{3515,303},{3516,304},{3517,305},{3518,306},{3519,307},{3520,308},{4000,309},{4001,310},{4002,311},{4003,312},{4008,313},{4009,314},{4010,315},{4011,316},{4012,317},{4013,318},{4014,319},{4015,320},{4016,321},{4017,322},{4018,323},{4019,324},{4020,325},{4021,326},{4022,327},{4023,328},{4024,329},{4500,330},{4501,331},{4502,332},{4525,333},{5000,334},{5001,335},{5002,336},{5003,337},{5500,338},{5501,339},{5502,340},{5503,341},{5504,342},{5505,343},{5506,344},{5507,345},{5508,346},{5509,347},{5510,348},{5512,349},{6000,350},{6001,351},{6002,352},{6003,353},{6004,354},{6005,355},{6006,356},{6007,357},{6008,358},{6009,359},{6010,360},{6011,361},{6012,362},{6013,363},{6014,364},{6015,365},{6016,366},{6017,367},{6018,368},{6019,369},{6020,370},{6021,371},{6022,372},{6023,373},{6024,374},{6025,375},{6026,376},{6027,377},{6028,378},{6029,379},{6500,380},{6501,381},{6502,382},{6506,383},{6508,384},{6509,385},{6510,386},{7000,387},{7001,388},{7002,389},{7003,390},{7004,391},{7005,392},{7006,393},{7007,394},{7008,395},{7009,396},{7010,397},{7011,398},{7012,399},{7013,400},{7014,401},{7015,402},{7016,403},{7017,404},{7018,405},{7019,406},{7020,407},{7021,408},{7022,409},{7023,410},{7024,411},{7025,412},{7026,413},{7029,414},{7033,415},{7034,416},{7035,417},{7036,418},{7037,419},{7038,420},{7500,421},{7501,422},{7502,423},{8000,424},{8500,425},{8501,426},{9000,427},{9001,428},{9002,429},{9005,430},{9008,431},{9010,432},{9500,433},{9501,434},{9502,435},{9503,436},{9504,437},{9505,438},{9506,439},{9507,440},{9508,441},{9509,442},{9510,443},{9511,444},{9512,445},{9519,446},{9520,447},{9522,448},{9524,449},{9525,450},{9527,451},{9528,452},{10000,453},{10001,454},{10003,455},{10004,456},{10005,457},{10006,458},{10007,459},{10008,460},{10009,461},{10010,462},{10011,463},{10012,464},{10013,465},{10014,466},{10015,467},{10016,468},{10017,469},{10018,470},{10019,471},{10020,472},{10021,473},{10022,474},{10023,475},{10024,476},{10025,477},{10026,478},{10027,479},{10500,480},{10501,481},{10502,482},{10503,483},{10504,484},{10505,485},{10506,486},{10507,487},
        // missing heads
        {26,46},{27,45},{28,16},{29,49},{30,4},{31,1},{32,7},{33,24},{34,53},{35,42},{36,28},{37,2},{38,9},{1028,137},{1029,142},{1529,190},{1537,184},{1538,205},{1539,195},{2030,245},{2031,255},{2519,268},{3506,306},{3508,294},{4004,324},{4005,323},{4006,318},{4007,315},{5511,340},{5513,338},{5514,423},{5515,349},{5516,428},{5517,486},{5518,453},{5519,341},{5520,338},{5521,339},{5522,403},{5523,433},{5524,438},{5526,421},{6031,363},{6032,359},{6033,357},{6503,478},{6504,476},{6505,479},{6507,479},{6511,156},{6512,483},{6513,156},{6514,482},{6516,468},{6517,466},{6519,468},{6522,479},{7027,470},{7030,341},{7032,364},{7039,457},{7041,399},{7042,449},{7043,396},{7048,411},{9003,363},{9004,388},{9006,340},{9009,449},{9011,344},{9012,359},{9014,351},{9015,350},{9016,393},{9017,453},{9018,387},{9019,340},{9024,343},{9027,390},{9514,374},{9515,372},{9516,340},{9517,339},{9518,354},{9521,345},{9523,430},{9526,338},{9529,342},{9530,343},{9531,355},{9532,350},{9536,422},{9538,352},{9539,437},{9540,483},{10028,359},{10029,389},{10030,477}
    };
    if (Utils::Contains(fifaHeadMapping, (UInt)fifaPlayer->internal.headtypecode))
        player->mAppearance.mGenericFace = fifaHeadMapping[fifaPlayer->internal.headtypecode];
    
    player->mShoeType.SetFromInt(GetPlayerShoesIdFromFifaId(isLastVersion ? fifaPlayer->internal.shoetypecode : 0));
}

void AppearanceGenerator::GenerateAppearanceDefs(Path const &dbPath, Path const &infoPath, Bool women) {
    enum FoomAppearanceParamType {
        FoomHairColor, FoomBeardColor, FoomSkinTone
    };
    using AppearanceParamCounter = Map<UInt, UInt>;
    Map<UInt, Map<UInt, AppearanceParamCounter>> appearanceFifam;
    Map<UInt, Map<UInt, AppearanceParamCounter>> appearanceFoom;
    Map<Int, Int> foomCountryIdToFifam;
    StringA gender = women ? "_women" : "";
    FifamDatabase::mReadingOptions.mReadPersons = false;
    FifamDatabase::mReadingOptions.mReadClubs = false;
    FifamDatabase::mReadingOptions.mReadCountryCompetitions = false;
    FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
    FifamDatabase db(13, "database_appearance" + gender);
    FifaDatabase::m_firstSupportedGameVersion = FifaDatabase::m_currentGameVersion;
    FifaDatabase fifaDb(dbPath / L"fifa");
    FifamReader countriesReader(infoPath / ("fifam_countries" + gender + ".txt"));
    if (countriesReader.Available()) {
        countriesReader.SkipLine();
        while (!countriesReader.IsEof()) {
            if (!countriesReader.EmptyLine()) {
                String d;
                Int FootballManagerID = -1, FIFAManagerReplacementID = -1;
                countriesReader.ReadLineWithSeparator(L'\t', d, d, d, d, FootballManagerID, d, d, d, FIFAManagerReplacementID);
                if (FootballManagerID != -1 && FIFAManagerReplacementID != -1)
                    foomCountryIdToFifam[FootballManagerID] = FIFAManagerReplacementID;
            }
            else
                countriesReader.SkipLine();
        }
    }
    FifamReader reader("fm_players_fifa_appearance.txt", 0);
    if (reader.Available()) {
        reader.SkipLine();
        while (!reader.IsEof()) {
            if (!reader.EmptyLine()) {
                String d;
                int	foomEthnicity = -1, foomHairColor = -1, foomSkinTone = -1, foomNation = -1, foomNationOfBirth = -1,
                    foomWearsHijab = 0, fifaId = -1;
                reader.ReadLineWithSeparator(L'\t', d, d, d, d, foomEthnicity, foomHairColor, d, foomSkinTone,
                    d, foomNation, foomNationOfBirth, foomWearsHijab, fifaId);
                if (foomEthnicity != -1 && fifaId > 0) {
                    FifaPlayer *fifaPlayer = fifaDb.GetPlayer(fifaId);
                    if (!fifaPlayer)
                        continue;
                    FifamCountry *appearanceCountry = nullptr;
                    if (foomNationOfBirth != -1 && Utils::Contains(foomCountryIdToFifam, foomNationOfBirth))
                        appearanceCountry = db.GetCountry(foomCountryIdToFifam[foomNationOfBirth]);
                    if (foomNation != -1 && !appearanceCountry && Utils::Contains(foomCountryIdToFifam, foomNation))
                        appearanceCountry = db.GetCountry(foomCountryIdToFifam[foomNation]);
                    if (!appearanceCountry)
                        continue;

                    static Map<UInt, FifamAppearanceDefs::Type> CountryMainAppearanceType;
                    if (CountryMainAppearanceType.empty()) {
                        // TODO: verify this
                        for (UInt countryId = 1; countryId <= FifamDatabase::NUM_COUNTRIES; countryId++) {
                            FifamCountry *country = db.GetCountry(countryId);
                            if (country && !country->mAppearanceData.empty()) {
                                UInt maxValue = 0;
                                FifamAppearanceDefs::Type type;
                                UInt prevValue = 0;
                                for (UInt i = 0; i < country->mAppearanceData.size(); i++) {
                                    UInt thisValue = country->mAppearanceData[i].mDistributionProbability - prevValue;
                                    if (thisValue > maxValue) {
                                        type = (FifamAppearanceDefs::Type)appearanceCountry->mAppearanceData[i].mAppearanceType;
                                        maxValue = thisValue;
                                    }
                                    prevValue = country->mAppearanceData[i].mDistributionProbability;
                                }
                                if (maxValue > 0)
                                    CountryMainAppearanceType[countryId] = type;
                            }
                        }
                    }

                    auto GetCountryMainAppearanceType = [](FifamCountry *country) {
                        if (Utils::Contains(CountryMainAppearanceType, country->mId))
                            return CountryMainAppearanceType[country->mId];
                        return (FifamAppearanceDefs::Type)0;
                    };

                    FifamAppearanceDefs::Type countryEthnicity = GetCountryMainAppearanceType(appearanceCountry);
                    FifamAppearanceDefs::Type ethnicity = countryEthnicity;

                    if (foomEthnicity == 0) { // Northern European
                        Bool isEasternEurope = countryEthnicity == FifamAppearanceDefs::EasternEuropeLight || countryEthnicity == FifamAppearanceDefs::EasternEuropeDark;
                        if (foomSkinTone <= 7)
                            ethnicity = isEasternEurope ? FifamAppearanceDefs::EasternEuropeLight : FifamAppearanceDefs::WesternEuropeLight;
                        else
                            ethnicity = isEasternEurope ? FifamAppearanceDefs::EasternEuropeDark : FifamAppearanceDefs::WesternEuropeDark;
                    }
                    else if (foomEthnicity == 1) { // Mediterranean/Hispanic
                        if (countryEthnicity == FifamAppearanceDefs::Latin)
                            ethnicity = FifamAppearanceDefs::Latin;
                        else if (foomSkinTone <= 5)
                            ethnicity = FifamAppearanceDefs::WesternEuropeLight;
                        else if (foomSkinTone <= 11)
                            ethnicity = FifamAppearanceDefs::WesternEuropeDark;
                        else
                            ethnicity = FifamAppearanceDefs::Latin;
                    }
                    else if (foomEthnicity == 2) { // North African/Middle Eastern
                        if (foomSkinTone <= 7)
                            ethnicity = FifamAppearanceDefs::Arabic2;
                        else
                            ethnicity = FifamAppearanceDefs::Arabic1;
                    }
                    else if (foomEthnicity == 3) { // African/Caribbean
                        if (foomSkinTone < 0 || foomSkinTone >= 16)
                            ethnicity = FifamAppearanceDefs::African1;
                        else
                            ethnicity = FifamAppearanceDefs::African2;
                    }
                    else if (foomEthnicity == 4) { // Asian
                        ethnicity = FifamAppearanceDefs::Indian;
                    }
                    else if (foomEthnicity == 5 || foomEthnicity == 10) { // South East Asian or East Asian
                        ethnicity = FifamAppearanceDefs::Asian;
                    }
                    else if (foomEthnicity == 6 || foomEthnicity == 8) { // Pacific Islander or Native Australian
                        if (foomSkinTone <= 5)
                            ethnicity = FifamAppearanceDefs::WesternEuropeLight;
                        else if (foomSkinTone <= 11)
                            ethnicity = FifamAppearanceDefs::WesternEuropeDark;
                    }
                    else if (foomEthnicity == 7) {  // Native American
                        ethnicity = FifamAppearanceDefs::Latin;
                    }
                    else if (foomEthnicity == 9) {  // Mixed Race
                        if (ethnicity == (FifamAppearanceDefs::Type)0)
                            continue;
                    }
                    else
                        continue;

                    auto &dataFoom = appearanceFoom[ethnicity];
                    auto &dataFifam = appearanceFifam[ethnicity];

                    if (foomSkinTone >= 0 && foomSkinTone <= 19) {
                        switch (foomSkinTone) {
                        case 0:
                        case 1:
                            dataFoom[FoomSkinTone][0]++;
                            break;
                        case 2:
                        case 3:
                            dataFoom[FoomSkinTone][1]++;
                            break;
                        case 4:
                        case 5:
                            dataFoom[FoomSkinTone][2]++;
                            break;
                        case 6:
                        case 7:
                            dataFoom[FoomSkinTone][3]++;
                            break;
                        case 8:
                        case 9:
                            dataFoom[FoomSkinTone][4]++;
                            break;
                        case 10:
                        case 11:
                            dataFoom[FoomSkinTone][5]++;
                            break;
                        case 12:
                        case 13:
                            dataFoom[FoomSkinTone][6]++;
                            break;
                        case 14:
                        case 15:
                            dataFoom[FoomSkinTone][7]++;
                            break;
                        case 16:
                        case 17:
                            dataFoom[FoomSkinTone][8]++;
                            break;
                        case 18:
                        case 19:
                            dataFoom[FoomSkinTone][9]++;
                            break;
                        }
                    }

                    if (!foomWearsHijab) {
                        if (foomHairColor >= 0 && foomHairColor <= 6) {
                            UInt hairColor = 0;
                            switch (foomHairColor) {
                            case 0: // Black
                                hairColor = FifamHairColor::Black;
                                break;
                            case 1: // Blonde
                                hairColor = FifamHairColor::Blonde;
                                break;
                            case 2: // Brown
                                hairColor = FifamHairColor::Darkbrown;
                                break;
                            case 3: // Grey
                                hairColor = 8;
                                break;
                            case 4: // Light Blonde
                                hairColor = FifamHairColor::Platinumblonde;
                                break;
                            case 5: // Light Brown
                                hairColor = FifamHairColor::Lightbrown;
                                break;
                            case 6: // Red
                                hairColor = FifamHairColor::Red;
                                break;
                            }
                            dataFoom[FoomHairColor][hairColor]++;
                            FifamPlayerAppearance playerAppearance;
                            playerAppearance.mHairColor = hairColor;
                            playerAppearance.SetBeardColorFromHairColor();
                            if (!women)
                                dataFoom[FoomBeardColor][playerAppearance.mBeardColor]++;
                        }
                    }

                    FifamPlayer player;
                    SetFromFifaPlayer(&player, fifaPlayer);

                    if (foomWearsHijab)
                        dataFifam[FifamAppearanceDefs::ParamHair][596]++;
                    else {
                        dataFifam[FifamAppearanceDefs::ParamHair][player.mAppearance.mHairStyle]++;
                        dataFifam[FifamAppearanceDefs::ParamHairColor][player.mAppearance.mHairColor]++;
                    }
                    if (!women) {
                        dataFifam[FifamAppearanceDefs::ParamBeard][player.mAppearance.mBeardType]++;
                        dataFifam[FifamAppearanceDefs::ParamBeardColor][player.mAppearance.mBeardColor]++;
                    }
                    dataFifam[FifamAppearanceDefs::ParamFace][player.mAppearance.mGenericFace]++;
                    dataFifam[FifamAppearanceDefs::ParamEyeColor][player.mAppearance.mEyeColour]++;
                    dataFifam[FifamAppearanceDefs::ParamSkinColor][player.mAppearance.mSkinColor]++;
                    dataFifam[FifamAppearanceDefs::ParamVariation][player.mAppearance.mFaceVariation]++;
                }
            }
            else
                reader.SkipLine();
        }
    }

    const UInt MIN_PLAYERS_COUNT = 2;

    for (auto &[type, d] : appearanceFifam)
        d[FifamAppearanceDefs::ParamSideburns][0] = MIN_PLAYERS_COUNT;
    if (women) {
        for (auto &[type, d] : appearanceFifam) {
            d[FifamAppearanceDefs::ParamBeard][FifamBeardType::None] = MIN_PLAYERS_COUNT;
            d[FifamAppearanceDefs::ParamBeardColor][FifamBeardColor::Black] = MIN_PLAYERS_COUNT;
        }
        for (auto &[type, d] : appearanceFoom)
            d[FoomBeardColor][FifamBeardColor::Black] = MIN_PLAYERS_COUNT;
    }

    auto TransateCountToWeight = [&](Map<UInt, Map<UInt, AppearanceParamCounter>> &data) {
        for (auto &[def, d] : data) {
            for (auto &[param, p] : d) {
                auto ShouldRemove = [&](const auto &kv) {
                    UInt id = kv.first;
                    UInt count = kv.second;
                    if (count < MIN_PLAYERS_COUNT)
                        return true;
                    return false;
                };
                for (auto it = p.begin(); it != p.end(); ) {
                    if (ShouldRemove(*it))
                        it = p.erase(it);
                    else
                        ++it;
                }
                UInt minValue = UINT32_MAX;
                UInt maxValue = 0;
                for (auto &[id, count] : p) {
                    minValue = Utils::Min(minValue, count);
                    maxValue = Utils::Max(maxValue, count);
                }
                for (auto &[id, count] : p)
                    count = (UChar)Utils::MapTo(count, minValue, maxValue, 1, 255);
            }
        }
    };

    TransateCountToWeight(appearanceFifam);
    TransateCountToWeight(appearanceFoom);

    StringA baseName = "AppearanceDefs" + gender;

    FifamAppearanceDefs &defs = db.mAppearanceDefs;
    // first - replace only heads and hairs
    Vector<FifamAppearanceDefs::Param> paramsFirst = { FifamAppearanceDefs::ParamFace, FifamAppearanceDefs::ParamHair };
    for (auto &e : defs.mDefs) {
        auto &def = e.second;
        for (auto p : paramsFirst) {
            def.mParameters[p].clear();
            def.mParametersSum[p] = 0;
            auto &data = appearanceFifam[e.first][p];
            for (auto [id, count] : data) {
                if (count > 0) {
                    def.mParameters[p].emplace_back(id, count);
                    def.mParametersSum[p] += count;
                }
            }
        }
    }
    defs.Write(baseName + "_generated.sav");
    // second - replace everything else
    for (auto &e : defs.mDefs) {
        auto &def = e.second;
        for (UInt p = 0; p < 9; p++) {
            def.mParameters[p].clear();
            def.mParametersSum[p] = 0;
            auto &data = appearanceFifam[e.first][p];
            for (auto [id, count] : data) {
                if (count > 0) {
                    def.mParameters[p].emplace_back(id, count);
                    def.mParametersSum[p] += count;
                }
            }
        }
    }
    defs.Write(baseName + "_generated_all.sav");
    // third - foom parameters
    for (auto &e : defs.mDefs) {
        auto &def = e.second;
        for (UInt p = 0; p < 9; p++) {
            UInt foomParam = -1;
            if (p == FifamAppearanceDefs::ParamSkinColor)
                foomParam = FoomAppearanceParamType::FoomSkinTone;
            else if (p == FifamAppearanceDefs::ParamHairColor)
                foomParam = FoomAppearanceParamType::FoomHairColor;
            else if (p == FifamAppearanceDefs::ParamBeardColor)
                foomParam = FoomAppearanceParamType::FoomBeardColor;
            if (foomParam != -1) {
                def.mParameters[p].clear();
                def.mParametersSum[p] = 0;
                auto &data = appearanceFoom[e.first][foomParam];
                for (auto [id, count] : data) {
                    if (count > 0) {
                        def.mParameters[p].emplace_back(id, count);
                        def.mParametersSum[p] += count;
                    }
                }
            }
        }
    }
    defs.Write(baseName + "_generated_all_foom.sav");
}
