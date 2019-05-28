#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include <math.h>
#include <limits.h>

void clearScreen()
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
	system("cls");
#endif
}

void pauseShell()
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	printf("Presione cualquier tecla para continuar...");
	int c = getchar();
#endif

#if defined(_WIN32) || defined(_WIN64)
	system("pause");
#endif
}

struct DogType{
	char name[32];
	char type[32];
	int age;
	char breed[16];
	int height;
	float weight;
	char sex;
	bool deleted;
	int index;
	int prevHashIndex;
	int medicalHistoryID;
};

char types[2][32] = {"Dog", "Cat"};
char sexes[2] = {'H', 'M'};

char dogBreed[10][32] = {"Alano", "Alaskan Malamute", "Boxer", "Bull Terrier ", "Chihuahua", "Chow Chow", "Samoyedo", "Pitbull", "Siberian Husky", "Dalmata"};
char catBreed[10][32] = {"Angora turco", "Bombay", "Himalayo", "Mau egipcio", "Serengeti", "Van Turco", "Khao Manee", "Chartreux", "Cymric", "Devon rex"};

char names[1914][32] = {"abby","ada", "addie", "adelaide", "adeline", "adelyn", "adriana", "agatha", "agnes", "alexia", "alexis", "alice", "allie", "alyssa", "amari", "ambe", "amelia", "amelie","amina", "angel", "angelica", "angie", "annabell", "annie", "anya", "ariel", "ashley", "asia", "aspen", "athena", "aubrey", "autumn", "ava", "avery", "baby", "bailey", "bambi", "basil", "bea", "bean", "bella", "bertha", "bessie", "betsy", "betty", "bianca", "billie", "bindi", "birdie", "biscuit", "blai", "blakely", "blanche", "blondie", "blossom", "bonnie", "brandy", "brielle", "brooklyn", "brownie", "brynn", "buffy", "cadence", "callie", "cameron", "camilia", "camille", "candy", "carla", "carly", "carmela", "carmen", "carolina", "carolyn", "casey", "cassie", "cecelia", "chance", "chanel", "charlie", "charlotte", "che", "cherry", "cheyenne", "chica", "chloe", "cinnamon", "clara", "clarissa", "clementine", "cleo", "clove", "coco", "cookie", "cora", "coraline", "courtney", "cricket", "crystal", "daisy", "dakota", "dallas", "dana", "daphne", "darla", "darlene", "delia", "delilah", "demi", "destiny", "diamond", "diva", "dixie", "dolly", "dolores", "dora", "dorothy", "dory", "dot", "dottie", "duchess", "eden", "edie", "edith", "effie", "electra", "elena", "eliza", "ella", "ellie", "eloise", "elsa", "elsie", "embe", "emery", "emma", "emmy", "enid", "erin", "esmerelda", "estelle", "esthe", "etta", "eudora", "eva", "evelyn", "evie", "faith", "fancy", "fannie", "fantasia", "faye", "felecity", "fern", "fifi", "fiona", "flo", "foxy", "frances", "frida", "gabby", "gemma", "genesis", "geneva", "georgia", "geraldine", "gia", "gidget", "gigi", "ginge", "giselle", "gloria", "goldie", "grace", "greta", "gretchen", "gwen", "gypsy", "hadley", "haley", "halle", "hallie", "hannah", "harley", "harlow", "harpe", "harriet", "hattie", "haven", "hazel", "heidi", "henley", "hershey", "holly", "honey", "hope", "ibby", "ida", "imani", "indigo", "inez", "ingrid", "irene", "iris", "isabella", "isla", "ivory", "ivy", "izzy", "jackie", "jada", "jade", "jamie", "jane", "janelle", "jasmine", "jenna", "jersey", "jessie", "jewel", "joan", "jolene", "jordan", "josie", "journey", "joy", "juliet", "june", "juno", "justice", "kali", "kallie", "kamila", "karina", "karla", "karma", "kate", "kay", "kayla", "kelly", "kelsey", "kendall", "kennedy", "kenya", "kiara", "kiki", "kimmie", "kinley", "kinsley", "kira", "kiwi", "koko", "kona", "lacy", "lady", "laika", "laney", "lassie", "layla", "leia", "lena", "leona", "leslie", "lexy", "liana", "libby", "liberty", "lila", "lily", "liv", "lizzy", "lois", "lola", "london", "lorelai", "loretta", "lorraine", "louise", "lovely", "lucia", "lucille", "lucinda", "lucky", "lucy", "lula", "lulu", "luna", "lydia", "lyla", "lyric", "mabel", "mackenzie", "macy", "maddie", "madeline", "madison", "mae", "maeve", "maggie", "maisie", "makayla", "malia", "mallory", "mandy", "mango", "maple", "mara", "margaret", "margot", "marianna", "marilyn", "mary", "matilda", "maude", "maya", "mazie", "mckenna", "megan", "melina", "melody", "mia", "mika", "mila", "miley", "millie", "mimi", "minnie", "miranda", "miriam", "missy", "mitzi", "mocha", "molly", "morgan", "moxie", "muffin", "mya", "myra", "nadia", "nala", "nanette", "naomi", "nell", "nellie", "nettie", "nia", "nikita", "nikki", "nina", "noel", "nola", "nora", "nori", "norma", "nova", "odessa", "olive", "olivia", "opal", "oreo", "paige", "paisley", "pandora", "paris", "parke", "patsy", "payton", "peach", "peaches", "peanut", "pearl", "pebbles", "peggy", "penelope", "penny", "peppe", "perdia", "phoenix", "phoebe", "pipe", "pippa", "pippy", "pixie", "polly", "poppy", "portia", "precious", "princess", "priscilla", "pumpkin", "queen", "quinn", "raven", "reese", "regina", "remy", "riley", "rio", "rita", "rive", "rosalie", "rose", "rosemary", "rosie", "rowan", "roxy", "ruby", "ruth", "ruthie", "rylie", "sadie", "sage", "sally", "sam", "sammie", "sandy", "sasha", "sassy", "savannah", "sawye", "scarlet", "scout", "selena", "selma", "serena", "serenity", "shadow", "sheba", "shelby", "shiloh", "shirley", "sienna", "sierra", "silvia", "sissy", "siti", "skye", "skyla", "smokey", "snickers", "snookie", "snowball", "sookie", "sophia", "sta", "starla", "stella", "storm", "sue", "suga", "suki", "summe", "sunny", "susannah", "susie", "sweetie", "sydney", "sylvia", "tabitha", "talia", "taylo", "tessa", "thea", "theo", "tiana", "tilly", "tootsie", "trixie", "trudy", "twinkie", "ursula", "valentine", "valeria", "velma", "venus", "vera", "victoria", "viola", "violet", "virginia", "vivi", "vivienne", "vixen", "whitney", "wiggles", "willa", "willow", "winifred", "winnie", "winte", "wren", "xena", "yasmin", "yvonne", "zara", "zelda", "zia", "zoe", "zola", "zuri", "abbott", "abe", "ace", "achilles", "aero", "aiden", "aj", "albert", "alden", "alex", "alexande", "alfie", "alford", "ali", "alista", "alvin", "amos", "andre", "andy", "angus", "apollo", "archie", "aries", "arnold", "arthu", "artie", "ash", "ashby", "atlas", "augustus", "austin", "axel", "bailey", "bandit", "barkley", "barney", "baron", "barry", "barton", "basil", "baxte", "beans", "bea", "beau", "benji", "bennett", "benny", "benson", "bentley", "bernard", "bertrand", "beta", "billy", "bingo", "blake", "blaze", "blue", "bo", "boome", "bowie", "bradford", "bradley", "brady", "braxton", "brigham", "brock", "broderick", "brody", "brogan", "bronson", "brownie", "bruce", "bruno", "brutus", "bryce", "bubba", "buck", "bucky", "buddy", "buste", "butch", "buzz", "byron", "cadence", "caesa", "cain", "calvin", "captain", "carlton", "carte", "cash", "caspe", "cecil", "cedric", "champ", "chance", "charlie", "chase", "cheste", "chewy", "chico", "chief", "chip", "chips", "cisco", "cj", "clarence", "clark", "clement", "clifford", "clyde", "coco", "cody", "colby", "coleman", "colin", "conrad", "coope", "corky", "cornelius", "cosmo", "crawford", "damien", "dane", "daniel", "dante", "darby", "darius", "davis", "davy", "denve", "dexte", "diego", "diesel", "dixon", "doc", "dodge", "domingo", "donnie", "donovan", "drew", "dudley", "duffy", "duke", "dusty", "dylan", "earl", "easton", "eddie", "edga", "edison", "edmund", "edward", "edwin", "eldon", "eli", "eliot", "elme", "elvis", "elway", "emerson", "emmett", "emory", "ernest", "eugene", "evan", "evande", "everett", "ezra", "fabio", "fang", "farley", "felix", "felton", "fenton", "fido", "finley", "finn", "fishe", "flash", "fletche", "floyd", "fonzi", "forrest", "foste", "francis", "frank", "frankie", "franklin", "freddy", "frederick", "freeman", "fritz", "gage", "george", "gerald", "ghost", "gilbert", "gizmo", "goose", "gordy", "grady", "graham", "griffin", "grove", "gunne", "gus", "hamilton", "hank", "harley", "harold", "harrison", "harry", "hart", "harvey", "hawkeye", "hayes", "heath", "hecto", "henry", "herbert", "herman", "hilton", "hogan", "home", "hooch", "hoove", "hoss", "huck", "hudson", "hugh", "hunte", "hyatt", "iggy", "igo", "ike", "indy", "ira", "irving", "ivan", "ives", "jack", "jackson", "jacob", "jag", "jake", "james", "jameson", "jaspe", "java", "jax", "jefferson", "jerry", "jesse", "jet", "jethro", "joey", "john", "johnny", "jonas", "joseph", "judge", "junio", "kane", "kendrick", "kent", "kilo", "king", "kobe", "koda", "kona", "lambert", "lance", "lane", "langston", "lars", "lawrence", "lemuel", "lenny", "leo", "leonard", "leroy", "levi", "lewis", "liam", "lincoln", "linus", "logan", "loki", "louie", "lucky", "luigi", "luke", "luthe", "mac", "maguire", "majo", "malcolm", "marco", "mario", "marley", "martin", "marty", "mason", "maverick", "max", "maximillian", "maximus", "maxwell", "mccoy", "mckinley", "merce", "meye", "michael", "mickey", "milburn", "miles", "mille", "milo", "milton", "moe", "mojo", "monroe", "moose", "morris", "mort", "morton", "murphy", "murray", "nacho", "napoleon", "nash", "ned", "nelson", "nero", "newton", "nico", "niles", "noah", "norm", "norman", "norris", "oakley", "odie", "odin", "olive", "ollie", "oreo", "orson", "osca", "otis", "otto", "overton", "owen", "ozzy", "pablo", "paco", "parke", "parley", "patch", "peanut", "peppe", "percival", "percy", "petey", "phillip", "pierce", "pipe", "pogo", "pongo", "porte", "prince", "quincy", "quinton", "rada", "ralph", "rambo", "randall", "range", "rascal", "raymond", "rebel", "reed", "reese", "reggie", "remy", "reuben", "rex", "richard", "ricky", "ride", "riley", "ringo", "ripley", "robert", "rocco", "rockwell", "rocky", "romeo", "roscoe", "roy", "rudolph", "rudy", "rufus", "rupert", "russell", "rusty", "sam", "sammy", "samson", "sarge", "saul", "sawye", "scooby", "scoote", "scout", "scrappy", "sebastian", "sedrick", "shadow", "shamus", "sheldon", "shelton", "sherman", "sherwood", "shiloh", "sigmund", "silas", "simba", "simon", "skip", "smokey", "snoopy", "socks", "sparky", "spence", "spike", "spot", "stanley", "stewie", "stitch", "storm", "stuart", "summe", "sylvan", "taco", "tank", "taylo", "taz", "teddy", "tesla", "thaddeus", "theo", "theodore", "tho", "thornton", "tille", "titus", "tj", "tobias", "toby", "todd", "tot", "toto", "trappe", "tripp", "tristan", "truman", "tucke", "tyle", "tyson", "ulysses", "vaughn", "vernon", "victo", "vince", "vincent", "vinnie", "virgil", "wade", "wallace", "wally", "walte", "warren", "watson", "wesley", "wheele", "wilbu", "william", "wilson", "winston", "woody", "wyatt", "wylie", "yogi", "yoshi", "yukon", "zane", "zeus", "ziggy", "calvin", "loqui", "sam", "golum", "legolas", "bende", "bony", "bendico", "benito", "benji", "axel", "balon", "bartolo", "baxte", "beatle", "bob", "bobby", "bochan", "ecco", "gag", "fanfarron", "bagatela", "pajarita", "coacain", "pillo", "vondalo", "beny", "aaron", "banero", "alan", "aldo", "alf", "andy", "angus", "anubis", "apolo", "bebeto", "bicho", "birko", "bizko", "blacky", "boky", "brando", "brandon", "exiliado", "corsario", "dscolo", "gitano", "recto", "capricho", "furia", "rayo", "ares", "ariel", "arnold", "aron", "aston", "asto", "athos", "atula", "atom", "aurelio", "blacy", "blak", "blanco", "blue", "blusy", "bruce", "brutus", "buck", "ladron", "opio", "griso", "bomba", "zorron", "rambo", "tornado", "bribon", "terry", "ben", "big", "miko", "olive", "doki", "lilo", "niko", "borrys", "loqui", "nico", "nilo", "noah", "noel", "nogo", "gamberro", "pluto", "bart", "alfile", "dick", "billy", "jack", "grano", "alex", "gino", "rufo", "max", "tedy", "toby", "milo", "atila", "ike", "justin", "michael", "mick", "lein", "lacky", "lagun", "laki", "laky", "hugo", "nino", "sam", "leo", "pino", "bob", "bolo", "helio", "goliat", "bill", "nelson", "nemo", "nero", "nervio", "ness", "nesto", "nevado", "newman", "nicky", "maki", "malik", "maltese", "mambo", "manchas", "milo", "lillo", "biagio", "eneas", "black", "pedro", "ocho", "paco", "osca", "pippo", "gringo", "memo", "tho", "toto", "boris", "wolf", "gara", "luca", "pol", "maty", "junio", "kim", "kin", "king", "kino", "kirk", "lay", "mas", "elvis", "gustav", "guita", "igo", "interludio", "teno", "twist", "jazz", "kaisse", "kamilo", "kan", "kapiton", "karim", "keico", "keiko", "ken", "kenny", "kent", "layo", "layon", "lazan", "lebrel", "leke", "dizzy", "gipsi", "blues", "barotono", "figaro", "fidelio", "samba", "tango", "coma", "bosquejo", "keny", "kenzo", "kero", "ket", "kevin", "kiam", "kido", "kike", "kiko", "kilo", "leonardo", "leopoldo", "leslie", "leste", "canalla", "patraca", "cartucho", "duende", "lucife", "diablo", "macho", "satanas", "vandalo", "zombie", "rufo", "mascota", "ashley", "buck", "chips", "donnie", "gibson", "zeus", "laika", "tawny", "pablo", "paco", "pancho", "paulo", "pedro", "belka", "strelka", "zorba", "toto", "benji", "buddy", "einstein", "bo", "flafy", "quito", "abejundio", "adams", "akon", "anki", "atahulpa", "ramon", "raul", "raymundo", "ricardo", "rico", "rio", "rodrigo", "roldan", "rafael", "ponce", "mamaviche", "mimi", "braunty", "buba", "baku", "edwa", "dino", "hastom", "alejo", "itzan", "matt", "jim", "pite", "jack", "kil", "loky", "lolo", "longo", "lora", "lord", "goofy", "pinpin", "titty", "marcial", "grinch", "pinly", "pompis", "pirulin", "pupitos", "perlitas", "chawchaw", "coqui", "smith", "fred", "arnold", "bruce", "jerry", "kiro", "kittos", "kiu", "kiubo", "klaus", "lotus", "luca", "lucas", "lucero", "lucho", "broly", "javi", "moophy", "kull", "jeik", "son", "turbus", "rex", "chichico", "pafi", "fill", "crilin", "kobok", "kobu", "kody", "koki", "koko", "kong", "konig", "kopi", "koto", "krak", "ludo", "luis", "luk", "luka", "luigui", "tanta", "muku", "sinka", "shiwi", "kirwin", "mirmi", "moquito", "chanka", "wira", "yura", "licrin", "budy", "axel", "gaspe", "ringo", "romeo", "rupert", "roco", "scooby", "sherlock", "salem", "manolo", "manuel", "marcelo", "marcial", "marco", "crypto", "devil", "volcon", "bucky", "chucky", "strougman", "leo", "lionel", "lolo", "angel", "axel", "gogo", "gordo", "gremlin", "paco", "pancho", "marcos", "mariano", "mario", "marques", "martin", "marlon", "payet", "lloris", "feki", "adil", "rami", "pipo", "kraus", "krong", "krusty", "kuki", "kurro", "kurt", "kurz", "kuss", "kust", "mateo", "max", "maximiliano", "maximo", "meco", "rabiot", "blade", "blaise", "sakho", "toliso", "laurent", "ada", "albert", "addy", "alexande", "adelaide", "alford", "adeline", "alista", "agatha", "alvin", "agnes", "archie", "alice", "arthu", "amelia", "ashby", "anastasia", "augustus", "annabell", "Barton", "annamae", "Basil", "anne", "Benji", "anna", "Bennett", "arabella", "Benny", "arlene", "Benson", "ashlyn", "Bernard", "astrid", "Bertrand", "audra", "Bradford", "ava", "Bradley", "averill", "Braxton", "Bea", "Brigham", "Beatrice", "Brock", "Bella", "Broderick", "Bernadette", "Brogan", "Bertha", "Bronson", "Bernice", "Buddy", "Bessie", "Byron", "Beth", "cadence", "Betty", "carlton", "Beverly", "carte", "Billie", "cecil", "Blanche", "cedric", "Bonnie", "charlie", "camille", "cheste", "carlotta", "clark", "cassandra", "clement", "catherine", "clifford", "cecelia", "clyde", "celeste", "coleman", "celia", "colin", "charlotte", "conrad", "chloe", "cornelius", "claire", "crawford", "clara", "daniel", "clarabelle", "darius", "clarissa", "davis", "clementine", "davy", "cleo", "dexte", "coco", "dixon", "constance", "domingo", "cora", "donnie", "coral", "earl", "daisy", "easton", "darlene", "edge", "delilah", "edison", "dolores", "edmund", "doris", "edward", "dorothea", "edwin", "dorothy", "eldon", "dot", "eli", "dottie", "eliot", "edith", "elme", "edna", "emerson", "effie", "emmett", "elaine", "emory", "eleano", "ernest", "eleanora", "eugene", "eloise", "evande", "eliza", "everett", "elizabeth", "ezra", "ella", "felix", "ellamae", "felton", "elsa", "fenton", "elsie", "fido", "emily", "finley", "emma", "finn", "enid", "fletche", "esme", "forrest", "esmerelda", "foste", "estelle", "francis", "estella", "frank", "esthe", "franklin", "ethel", "frederick", "etta", "freeman", "eudora", "george", "eva", "gerald", "evelyn", "gilbert", "fannie", "grady", "felicity", "graham", "fern", "griffin", "florence", "hamilton", "frances", "harold", "frida", "harrison", "gail", "harry", "garnet", "hart", "gemma", "harvey", "geneva", "hayes", "genevieve", "heath", "georgetta", "hecto", "georgia", "henry", "geraldine", "herbert", "gertrude", "hilton", "gidget", "hoove", "ginge", "hudson", "gladys", "hugh", "gloria", "hunte", "glynn", "hyatt", "grace", "ike", "gracelyn", "ira", "greta", "irving", "gretchen", "ives", "gwendolyn", "jack", "harpe", "jacob", "harriet", "james", "hattie", "jameson", "hazel", "jaspe", "hedy", "jefferson", "helen", "john", "holly", "jonas", "ida", "joseph", "irene", "kendrick", "iris", "kent", "isabella", "lambert", "isadora", "lance", "ivy", "lane", "jacqueline", "langston", "jane", "lawrence", "joan", "lemuel", "jocelyn", "leo", "josephine", "leonard", "joy", "leroy", "joyce", "lewis", "judith", "liam", "julia", "lincoln", "juliana", "llewellyn", "june", "luthe", "katherine", "mack", "kay", "maguire", "kitty", "malcolm", "lady", "martin", "lassie", "mason", "layne", "maverick", "leona", "max", "lila", "maximilian", "lilibeth", "maxwell", "lillian", "mckinley", "lily", "merce", "lizette", "meye", "lois", "michael", "lola", "milburn", "loretta", "miles", "lorraine", "mille", "louisa", "milo", "louise", "milton", "lucia", "monroe", "lucille", "morton", "lucinda", "murphy", "lucy", "murray", "luella", "napoleon", "lula", "nelson", "mabel", "newton", "madeline", "niles", "mae", "norman", "maisie", "norris", "mamie", "oakley", "mara", "olive", "margaret", "orson", "marge", "osca", "marianne", "otis", "marilyn", "otto", "marjorie", "overton", "maryella", "owen", "matilda", "park", "maude", "parke", "maura", "parley", "may", "percival", "mazie", "percy", "melanie", "phillip", "melissa", "pierce", "melody", "porte", "meredith", "quincy", "minnie", "quinton", "miranda", "ralph", "missy", "randall", "molly", "raymond", "myra", "reed", "myrtle", "reuben", "nanette", "rex", "nellie", "richard", "nettie", "robert", "nora", "rudolph", "norma", "rufus", "olive", "rupert", "olivia", "russell", "opal", "samson", "patsy", "saul", "pearl", "scout", "peggy", "sebastian", "penelope", "sedrick", "phoebe", "sheldon", "polly", "shelton", "posey", "sherman", "phyllis", "sherwood", "priscilla", "sigmund", "prudence", "silas", "queenie", "spot", "quinn", "stanley", "quinnie", "sterling", "rachel", "stetson", "rebecca", "stuart", "regina", "summe", "rhea", "sylvan", "rosalie", "taylo", "rose", "teddy", "rosemary", "thaddeus", "roxy", "theodore", "ruby", "thornton", "ruth", "thurston", "sadie", "tillman", "samantha", "tobias", "selma", "toby", "shirley", "todd", "sophia", "tristan", "sophronia", "truman", "stella", "tucke", "susannah", "tyle", "sue", "ulysses", "susie", "vaughn", "sylvia", "vernon", "tabitha", "victo", "theresa", "vincent", "tillie", "virgil", "ursula", "wade", "velma", "wallace", "vera", "walte", "victoria", "walton", "viola", "warren", "violet", "watson", "virgie", "weldon", "virginia", "wesley", "wanda", "weston", "waverly", "wheele", "willa", "wilbu", "williemae", "william", "wilma", "willis", "winifred", "wilson", "yvonne", "winston", "zelda", "wyatt", "zora", "wylie"};

void generateRegister(struct DogType *, int);
void getRegName(struct DogType *);
void getRegBreed (struct DogType *, char);

int SIZE = 1999;
int lastHashIndex[2000];

unsigned int calculateHash(const char *word)
{
	unsigned int hash = 0, c;

	for (size_t i = 0; word[i] != '\0'; i++){
		c = (unsigned char)word[i];
		hash = (hash << 3) + (hash >> (sizeof(hash) * CHAR_BIT - 3)) + c;
	}
	return hash % SIZE;
}

void generateRegister(struct DogType *sample, int currentIndex){

	strcpy(sample->name, names[(rand() % 1913)]);

	int typeNumber = rand() % 2 + 1;
	strcpy(sample->type, types[typeNumber - 1]);

	sample->age = rand() % 19;

	if (typeNumber == 1){
		strcpy(sample->breed, dogBreed[(rand() % 9)]);
	}
	else{
		strcpy(sample->breed, catBreed[(rand() % 9)]);
	}

	float weightNumber;
	if(typeNumber == 1){
		weightNumber = 4.0 + ((float)rand() / RAND_MAX)*(60.0-4.0);
		sample->height = rand() % 120 + 20;
	}else{
		weightNumber = 3.5 + ((float)rand() / RAND_MAX)*(19.0-3.5);
		sample->height = rand() % 45 + 10;
	}

	sample->weight = roundf(weightNumber * 100) / 100;

	sample->sex = sexes[(rand() % 2 + 1)-1];

	sample->deleted = false;
	sample->index = currentIndex;

	int hash = calculateHash(sample->name);

	sample->prevHashIndex = lastHashIndex[hash];
	lastHashIndex[hash] = currentIndex;
	sample->medicalHistoryID = -1;
}

int writeRegisterToCreate(void* ap, FILE* f, int index){
	struct DogType* dato;
	dato = ap;
	int d = fseek(f, index*sizeof(struct DogType), SEEK_SET);
	if( d == -1){
		printf("error al mover al index\n");
	}
	int r = fwrite(dato, sizeof(struct DogType), 1, f);

	if(r == 0){
		perror("Could not write Struct");
		exit(-1);
	}
	
	return 0;
}

void findByIndex(struct DogType *ap, int index){
	FILE* f;

	struct DogType reg;
	f = fopen("dataDogs.dat", "rb+");

	if(f == NULL){
		perror("Could not open a file");
		exit(-1);
	}
	int d = fseek(f, index*sizeof(struct DogType), SEEK_SET);
	if( d == -1){
		printf("error al mover al index\n");
	}
	int r = fread(&reg, sizeof(struct DogType), 1, f);
	if (r == 0){
        perror("Could no read structure");
        exit(-1);
    }

	strcpy(ap->name, reg.name);
	strcpy(ap->type, reg.type);
	ap->age = reg.age;
	strcpy(ap->breed, reg.breed);
	ap->height = reg.height;
	ap->weight = reg.weight;
	ap->sex = reg.sex;
	ap->deleted = reg.deleted;
	ap->index = reg.index;
	ap->prevHashIndex = reg.prevHashIndex;

	fclose(f);
}

int countRecords(){
	FILE* f;
	f = fopen("dataDogs.dat", "ab+");

	if(f == NULL){
		perror("Could not open a file");
		exit(-1);
	}
    int r;
    //r = fseek(f,0*sizeof( struct DogType ), SEEK_SET);
    if(r == -1){
		printf("error al mover al index\n");
    }
    struct DogType perro={ "", "",  0, "", 0, 0.0, 'f', 0, 0 };
    int count =0;
    while(fread(&perro, sizeof(struct DogType), 1, f )!=0){
        count++;
    }
	fclose(f);
    return count;
}
	
void writeHash(){
	int status = remove("hash.dat");
	if(status == 0 ){
		printf("Hash file deleted\n");
	}
 
	FILE *f;
	f = fopen("hash.dat", "ab+");

	if (f == NULL){
		perror("Could not open file");
		exit(-1);
	}

	int r = fwrite(lastHashIndex, sizeof(lastHashIndex), 1, f);

	if (r == 0){
		perror("Could not write Struct");
		exit(-1);
	}
	fclose(f);
}

void readHash(){
	FILE *f;
	f = fopen("hash.dat", "rb");
	int d = fseek(f, 0*sizeof(struct DogType), SEEK_SET);
	if( d == -1){
		printf("error al mover al index\n");
	}
	int test[2000];

	if (f == NULL){
		perror("Could not open file");
		exit(-1);
	}

	int r = fread(test, sizeof(test), 1, f);

	if (r == 0){
		perror("Could not read Struct hash");
		exit(-1);
	}

	for (int i = 0; i < 2000; i++){
		printf(" %d", test[i]);
	}
	
	fclose(f);
}

int registros;
int main(int argc, char *argv[]) {
	//para crear registros nuevos borrar archivo, cambiar rb+ a ab+ en fopen
	//PARA GENERAR DATA
	clearScreen();
	printf("---------------------------------------------------------------------------\n");
	printf("GENERANDO DATOS...\n");
	printf("---------------------------------------------------------------------------\n\n");

	srand(time(NULL));

	int status = remove("dataDogs.dat");
	if(status == 0 ){
		printf("Structures file deleted\n");
	}
	
	for (int i = 0; i < 2000; i++){
		lastHashIndex[i] = -1;
	}

	FILE* f;
	f = fopen("dataDogs.dat", "ab+");
	if(f == NULL){
		perror("Could not open a file");
		exit(-1);
	}

	struct DogType sample;

	for(int i = 0; i < 10000000; i++){
		generateRegister(&sample, i);
		int write =  writeRegisterToCreate(&sample, f, i);
	}

	fclose(f);

	writeHash();
	//readHash();
	
	struct DogType test;

	findByIndex(&test, 9999999);

	printf("\n\ntest ->");
	printf("\nname: %s\n", test.name);
	printf("type: %s\n", test.type);
	printf("age: %d\n", test.age);
	printf("breed: %s\n", test.breed);
	printf("height: %d\n", test.height);
	printf("weight: %.2f\n", test.weight);
	printf("sex: %c\n", test.sex);
	printf("index: %d\n", test.index);
	printf("prev hash index: %d\n\n", test.prevHashIndex);
	//printf("\nHay %d registros\n", countRecords());
	/*
	//PARA HACER DEBUGG

	struct DogType* sampleRead;
	registros = countRecords();
	printf("Ahora hay %d registros\n", registros);

	for (int i = 0; i < SIZE; i++){
		printf(" %d", lastHashIndex[i]);
	}

	struct DogType test;
	
	findByIndex(&test, 14);

	printf("name: %s\n", test.name);
	printf("index: %d\n", test.index);
	printf("prev hash index: %d\n\n", test.prevHashIndex);

	*/
	pauseShell();
	clearScreen();
	return 0;
}
