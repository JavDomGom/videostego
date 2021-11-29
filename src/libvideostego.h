#ifndef LIBVIDEOSTEGO_H
#define LIBVIDEOSTEGO_H

#include <unistd.h>
#include <string.h>

// All keywords known to refer to container types in MP4 files.
enum Types
{
	ainf = 0x61696e66, albm = 0x616c626d, auth = 0x61757468, avcn = 0x6176636e,
	bloc = 0x626c6f63, bpcc = 0x62706363, buff = 0x62756666, bxml = 0x62786d6c,
	ccid = 0x63636964, cdef = 0x63646566, chap = 0x63686170, clip = 0x636c6970,
	clsf = 0x636c7366, cmap = 0x636d6170, co64 = 0x636f3634, colr = 0x636f6c72,
	cprt = 0x63707274, crgn = 0x6372676e, crhd = 0x63726864, cslg = 0x63736c67,
	ctab = 0x63746162, ctts = 0x63747473, cvru = 0x63767275, dcfD = 0x64636644,
	dinf = 0x64696e66, dref = 0x64726566, drm  = 0x64726d20, dscp = 0x64736370,
	dsgd = 0x64736764, dstg = 0x64737467, edts = 0x65647473, elst = 0x656c7374,
	feci = 0x66656369, fecr = 0x66656372, fiin = 0x6669696e, fire = 0x66697265,
	fpar = 0x66706172, frma = 0x66726d61, fry  = 0x66727920, ftyp = 0x66747970,
	gitn = 0x6769746e, gnre = 0x676e7265, grpi = 0x67727069, hdlr = 0x68646c72,
	hmhd = 0x686d6864, hpix = 0x68706978, icnu = 0x69636e75, ID32 = 0x49443332,
	idat = 0x69646174, ihdr = 0x69686472, iinf = 0x69696e66, iloc = 0x696c6f63,
	imap = 0x696d6170, imif = 0x696d6966, infu = 0x696e6675, iods = 0x696f6473,
	iphd = 0x69706864, ipmc = 0x69706d63, ipro = 0x6970726f, iref = 0x69726566,
	jp2c = 0x6a703263, jp2h = 0x6a703268, jp2i = 0x6a703269, kmat = 0x6b6d6174,
	kywd = 0x6b797764, loci = 0x6c6f6369, load = 0x6c6f6164, lrcu = 0x6c726375,
	m7hd = 0x6d376864, matt = 0x6d617474, mdat = 0x6d646174, mdhd = 0x6d646864,
	mdia = 0x6d646961, mdri = 0x6d647269, meco = 0x6d65636f, mehd = 0x6d656864,
	mere = 0x6d657265, meta = 0x6d657461, mfhd = 0x6d666864, mfra = 0x6d667261,
	mfro = 0x6d66726f, minf = 0x6d696e66, mjhd = 0x6d6a6864, moof = 0x6d6f6f66,
	moov = 0x6d6f6f76, mvcg = 0x6d766367, mvci = 0x6d766369, mvex = 0x6d766578,
	mvhd = 0x6d766864, mvra = 0x6d767261, nmhd = 0x6e6d6864, ochd = 0x6f636864,
	odaf = 0x6f646166, odda = 0x6f646461, odhd = 0x6f646864, odhe = 0x6f646865,
	odrb = 0x6f647262, odrm = 0x6f64726d, odtt = 0x6f647474, ohdr = 0x6f686472,
	PICT = 0x50494354, padb = 0x70616462, paen = 0x7061656e, pclr = 0x70636c72,
	pdin = 0x7064696e, perf = 0x70657266, pitm = 0x7069746d, pnot = 0x706e6f74,
	prfl = 0x7072666c, resc = 0x72657363, resd = 0x72657364, rtng = 0x72746e67,
	sbgp = 0x73626770, schi = 0x73636869, schm = 0x7363686d, scpt = 0x73637074,
	sdep = 0x73646570, sdhd = 0x73646864, sdtp = 0x73647470, sdvp = 0x73647670,
	segr = 0x73656772, senc = 0x73656e63, sgpd = 0x73677064, sidx = 0x73696478,
	sinc = 0x73796e63, sinf = 0x73696e66, skip = 0x736b6970, smhd = 0x736d6864,
	srmb = 0x73726d62, srmc = 0x73726d63, srpp = 0x73727070, ssrc = 0x73737263,
	stbl = 0x7374626c, stco = 0x7374636f, stdp = 0x73746470, sthd = 0x73746864,
	stsc = 0x73747363, stsd = 0x73747364, stsh = 0x73747368, stss = 0x73747373,
	stsz = 0x7374737a, stts = 0x73747473, styp = 0x73747970, stz2 = 0x73747a32,
	subs = 0x73756273, swtc = 0x73777463, tfad = 0x74666164, tfhd = 0x74666864,
	tfma = 0x74666d61, tfra = 0x74667261, tibr = 0x74696272, tiri = 0x74697269,
	titl = 0x7469746c, tkhd = 0x746b6864, tmcd = 0x746d6364, traf = 0x74726166,
	trak = 0x7472616b, tref = 0x74726566, trex = 0x74726578, trgr = 0x74726772,
	trik = 0x7472696b, trun = 0x7472756e, tsel = 0x7473656c, udta = 0x75647461,
	uinf = 0x75696e66, UITS = 0x55495453, ulst = 0x756c7374, uuid = 0x75756964,
	vmhd = 0x766d6864, vwdi = 0x76776469, wide = 0x77696465, yrrc = 0x79727263
};

// All keywords known to refer to container sub-types in MP4 files.
enum SubTypes
{
	avc1 = 0x61766331, iso2 = 0x69736f32, isom = 0x69736f6d, mmp4 = 0x6d6d7034,
	mp41 = 0x6d703431, mp42 = 0x6d703432, mp71 = 0x6d703731, msnv = 0x6d736e76,
	ndas = 0x6e646173, ndsc = 0x6e647363, ndsh = 0x6e647368, ndsm = 0x6e64736d,
	ndsp = 0x6e647370, ndss = 0x6e647373, ndxc = 0x6e647863, ndxh = 0x6e647868,
	ndxm = 0x6e64786d, ndxp = 0x6e647870, ndxs = 0x6e647873
};

struct TypeData typeRecognition (int type);
char *subTypeRecognition (int subType);
unsigned int getFileSize (FILE*);
int getNumBlocks (FILE*, unsigned int, unsigned int);
unsigned int *getBlocksOffsets (FILE*, unsigned int*, unsigned int, unsigned int);
void printBlockInfo (FILE*, unsigned int, int, int, unsigned int);
void processHeader (FILE*, unsigned int, unsigned int);
void writeData (FILE*, unsigned int, unsigned int, char*);
void readData (FILE*, unsigned int);
void abracadabra (FILE*, char, unsigned int, unsigned int, char*);
void processFile (FILE*, char, char*);


#endif  /* LIBVIDEOSTEGO_H */
