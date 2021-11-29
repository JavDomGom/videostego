#include "libcommon.h"
#include "libvideostego.h"

struct TypeData {
    char *name;
    char *description;
};

/*
 * Function: typeRecognition
 * ----------------------------
 *   Returns a structure with the name and description of a recognized block type.
 *   
 *   Params:
 *      type: Integer number that represents the type to identify.
 *
 *   Return: Struct with block type data.
 */
struct TypeData typeRecognition (int type)
{
    struct TypeData typeData;

	typeData.name = "Other";
    typeData.description = "Unknown";

	switch (type)
    {
        case ctts:
            typeData.name = "ctts";
            typeData.description = "(Composition) time to sample.";
            break;
        case dinf:
            typeData.name = "dinf";
            typeData.description = "Data information box, container.";
            break;
		case drm:
            typeData.name = "drm ";
            typeData.description = "DRM container.";
            break;
        case ftyp:
            typeData.name = "ftyp";
            typeData.description = "File type and compatibility.";
            break;
        case 0x66726565: // "free" can't be configured as enum because is a reserved word.
            typeData.name = "free";
            typeData.description = "Free space.";
            break;
        case edts:
            typeData.name = "edts";
            typeData.description = "Edit list container.";
            break;
        case hdlr:
            typeData.name = "hdlr";
            typeData.description = "Handler, declares the media (handler) type.";
            break;
        case iods:
            typeData.name = "iods";
            typeData.description = "Object Descriptor container box.";
            break;
        case mdat:
            typeData.name = "mdat";
            typeData.description = "Media data container.";
            break;
        case mdhd:
            typeData.name = "mdhd";
            typeData.description = "Media header, overall information about the media.";
            break;
        case mdia:
            typeData.name = "mdia";
            typeData.description = "Container for the media information in a track.";
            break;
        case meta:
            typeData.name = "meta";
            typeData.description = "Metadata container.";
            break;
        case minf:
            typeData.name = "minf";
            typeData.description = "Media information container.";
            break;
		case moov:
            typeData.name = "moov";
            typeData.description = "Container for all the meta-data.";
            break;
		case mvhd:
            typeData.name = "mvhd";
            typeData.description = "Movie header, overall declarations.";
            break;
        case stbl:
            typeData.name = "stbl";
            typeData.description = "Sample table box, container for the time/space map.";
            break;
        case stsd:
            typeData.name = "stsd";
            typeData.description = "Sample descriptions (codec types, initialization etc.).";
            break;
        case stts:
            typeData.name = "stts";
            typeData.description = "(Decoding) time-to-sample.";
            break;
        case stss:
            typeData.name = "stss";
            typeData.description = "Sync sample table (random access points).";
            break;
        case stsc:
            typeData.name = "stsc";
            typeData.description = "Sample-to-chunk, partial data-offset information.";
            break;
        case stsz:
            typeData.name = "stsz";
            typeData.description = "Sample sizes (framing).";
            break;
        case stco:
            typeData.name = "stco";
            typeData.description = "Chunk offset, partial data-offset information.";
            break;
        case smhd:
            typeData.name = "smhd";
            typeData.description = "Sound media header, overall information (sound track only).";
            break;
        case sdtp:
            typeData.name = "sdtp";
            typeData.description = "Independent and Disposable Samples Box.";
            break;
        case sgpd:
            typeData.name = "sgpd";
            typeData.description = "Sample group definition box.";
            break;
        case sbgp:
            typeData.name = "sbgp";
            typeData.description = "Sample to Group box.";
            break;
        case tkhd:
            typeData.name = "tkhd";
            typeData.description = "Track header, overall information about the track.";
            break;
        case trak:
            typeData.name = "trak";
            typeData.description = "Container for an individual track or stream.";
            break;
        case udta:
            typeData.name = "udta";
            typeData.description = "User data.";
            break;
        case vmhd:
            typeData.name = "vmhd";
            typeData.description = "Video media header, overall information (video track only).";
            break;
        default:
            break;
    }

	return typeData;
}

/*
 * Function: subTypeRecognition
 * ----------------------------
 *   Returns the subtype of the first block in the file.
 *   
 *   Params:
 *      subType: Integer number that represents the subtype to identify.
 *
 *   Return: String with the name of the subtype.
 */
char *subTypeRecognition (int subType)
{
	char *subTypeName = "Other";

    switch (subType)
    {
        case mmp4:
            subTypeName = "mmp4"; break;
        case mp42:
			subTypeName = "mp42"; break;
        default:
            break;
    }

	return subTypeName;
}

/*
 * Function: getFileSize
 * ----------------------------
 *   Returns the total size of a file.
 *   
 *   Params:
 *      file: Open file object.
 *
 *   Return: Unsigned integer representing the total size of the file in Bytes.
 */
unsigned int getFileSize (FILE *file)
{
    fseek (file, 0L, SEEK_END);
    return ftell (file);
}

/*
 * Function: getNumBlocks
 * ----------------------------
 *   Returns the total number of blocks that exist between two offsets in the file.
 *   
 *   Params:
 *      file: Open file object.
 *      from: Starting offset address.
 *      to: Ending offset address.
 *
 *   Return: Integer representing the number of blocks.
 */
int getNumBlocks (FILE *file, unsigned int from, unsigned int to)
{
    int numBlocks = 0;
    unsigned char buffer[4];

    unsigned int blockSize;

	fseek (file, from, SEEK_SET);

    while (from != to)
    {
        fread (buffer, sizeof (buffer), 1, file); // Read & advance 4 bytes.
        blockSize = (buffer[0]<<24) | (buffer[1]<<16) | (buffer[2]<<8) | buffer[3];

        from += blockSize;
        
        fseek (file, from, SEEK_SET);
        
        numBlocks++;
    }

    return numBlocks;
}

/*
 * Function: getBlocksOffsets
 * ----------------------------
 *   Returns an array of addresses for each block that exists between two addresses.
 *   
 *   Params:
 *      file: Open file object.
 *      arr: Empty array to fill.
 *      from: Starting offset address.
 *      to: Ending offset address.
 *
 *   Return: Array of unsigned integers.
 */
unsigned int *getBlocksOffsets (FILE *file, unsigned int *arr, unsigned int from, unsigned int to)
{
    int i = 0;
	unsigned int fileSize = getFileSize (file);
    unsigned char buffer[4];

    unsigned int blockSize;

	fseek (file, from, SEEK_SET);

    while (from != fileSize)
    {
        arr[i] = from;
        i ++;

        fread (buffer, sizeof (buffer), 1, file); // Read & advance 4 bytes.
        blockSize = (buffer[0]<<24) | (buffer[1]<<16) | (buffer[2]<<8) | buffer[3];

        from += blockSize;

        fseek (file, from, SEEK_SET);
    }

    return arr;
}

/*
 * Procedure: printBlockInfo
 * ----------------------------
 *   Prints a tree with the hierarchy of the blocks in a file. It includes basic information such as the
 *   offset address at which each block begins, size in Bytes, and a brief description of the block.
 *   
 *   Params:
 *      file: Open file object.
 *      blockOffset: Offset of block to inspect.
 *      currentBlockNumber: Current block number.
 *      numBlocks: Total number of blocks to inspect.
 *      blockLevel: Integer representing the level of block.
 */
void printBlockInfo (FILE *file, unsigned int blockOffset, int currentBlockNumber, int numBlocks, unsigned int blockLevel)
{
    unsigned char buffer[4];
    unsigned int blockSize, type;

    fseek (file, blockOffset, SEEK_SET);

    fread (buffer, sizeof (buffer), 1, file); // Read & advance 4 bytes.
    blockSize = (buffer[0]<<24) | (buffer[1]<<16) | (buffer[2]<<8) | buffer[3];

    fread (buffer, sizeof (buffer), 1, file); // Read & advance 4 bytes.
    type = (buffer[0]<<24) | (buffer[1]<<16) | (buffer[2]<<8) | buffer[3];

    struct TypeData typeData = typeRecognition (type);
    int times = (blockLevel - 2) + 3 * (blockLevel - 1);

    // Draw hierarchical tree lines.
    switch (blockLevel)
    {
    case 0:
        break;
    case 1:
        if (currentBlockNumber == 1) printf ("\u2502\n");
        printf ("\u251c\u2500\u2500\u2500");
        break;
    default:
        if (currentBlockNumber == 1) printf ("\u2502%*s\u2502\n", times, " ");
        printf ("\u2502%*s", times, " ");
        printf ("%s\u2500\u2500\u2500", currentBlockNumber == numBlocks ? "\u2514" : "\u251c");
        break;
    }

    printf ("[%s] (0x%08x) %7u bytes: %s\n", typeData.name, blockOffset, blockSize, typeData.description);

    if (type == moov || type == trak || type == mdia ||
        type == minf || type == stbl || type == udta)
    {
        blockLevel += 1;
        numBlocks = getNumBlocks (file, blockOffset + 8, blockOffset + blockSize);

        unsigned int arr[numBlocks];
        unsigned int from = blockOffset + 8;
        unsigned int to = blockOffset + blockSize;
        unsigned int *blocks = getBlocksOffsets (file, arr, from, to);

        for (int i=0, currentBlockNumber = 1;i < numBlocks;i++, currentBlockNumber++)  
        {
            printBlockInfo (file, blocks[i], currentBlockNumber, numBlocks, blockLevel);
        }
    }
}

/*
 * Procedure: processHeader
 * ----------------------------
 *   This procedure establishes and writes very important information about the bytes to be modified at a
 *   specific point in the file. It also records a 4-byte reserved word to identify if the file has already
 *   been modified with this program.
 *   
 *   Params:
 *      file: Open file object.
 *      startFrom: Starting offset address.
 *      msgSize: Total size of the message.
 */
void processHeader (FILE *file, unsigned int startFrom, unsigned int msgSize)
{
    const unsigned int size = 2;
    const unsigned int type = 32;

    unsigned int totalSize = size + type + msgSize;
    unsigned int readedByte;
    unsigned char b1[1], newByte;
    char totalSizeHexChar[9];
    char byteChar[3];
    
    fseek (file, startFrom, SEEK_SET);

    sprintf (totalSizeHexChar, "%04x", totalSize);

    for (int i=0;i<4;i+=2)
    {
        strncpy (byteChar, &totalSizeHexChar[i], 2);
        byteChar[2] = '\0';
        
        newByte = (int) strtol (byteChar, NULL, 16);

        fwrite (&newByte, sizeof (newByte), 1, file); // Re-write the current byte & advance 1 byte.
    }

    char binHeaderType[4][9] = {
        "01110110", // v
        "01110011", // s
        "01110100", // t
        "01100111"  // g
    };

    for (int i=0;i<4;i++)
    {
        for (int c=0;c<8;c++)
        {
            char binFlag = binHeaderType[i][c];

            fread (b1, sizeof (b1), 1, file); // Read & advance 1 byte.

            readedByte = b1[0];
            
            if ((readedByte % 2 == 0 && binFlag == '1') || (readedByte % 2 != 0 && binFlag == '0'))
            {
                if (readedByte == 0xff)
                {
                    newByte = readedByte - 1;
                } else {
                    newByte = readedByte + 1;
                }

                fseek (file, -1, SEEK_CUR); // Backward 1 byte from the current position.
                fwrite (&newByte, sizeof (newByte), 1, file); // Re-write the current byte & advance 1 byte.
            }
        }
    }
}

/*
 * Procedure: writeData
 * ----------------------------
 *   This procedure modifies the appropriate bytes in the file based on the content and length of the message.
 *   
 *   Params:
 *      file: Open file object.
 *      startFrom: Starting offset address.
 *      msgSize: Total size of the message.
 *      msg: Message itself.
 */
void writeData (FILE *file, unsigned int startFrom, unsigned int msgSize, char *msg)
{   
    processHeader (file, startFrom, msgSize);

    startFrom += 2 + 32; // startFrom + size + type

    fseek (file, startFrom, SEEK_SET);

    unsigned char b1[1], newByte;
    unsigned int readedByte;
    char *binChar;

    for (int i=0;i<strlen (msg);i++)
    {
        binChar = intToBin ((unsigned int) msg[i]);

        for (int c=0;c<sizeof (binChar);c++)
        {
            char binFlag = binChar[c];

            fread (b1, sizeof (b1), 1, file); // Read & advance 1 byte.
        
            readedByte = b1[0];

            if ((readedByte % 2 == 0 && binFlag == '1') || (readedByte % 2 != 0 && binFlag == '0'))
            {
                // Decide to increase or decrease the current byte by one bit.
                if (readedByte == 0xff)
                {
                    newByte = readedByte - 1;
                } else {
                    newByte = readedByte + 1;
                }

                fseek (file, -1, SEEK_CUR); // Backward 1 byte from the current position.
                fwrite (&newByte, sizeof (newByte), 1, file); // Re-write the current byte & advance 1 byte.
            }
        }
    }
}

/*
 * Procedure: readData
 * ----------------------------
 *   This procedure checks if the file has been modified by this program. In that case it gets the number of
 *   bytes it has to read and processes them to get the original message.
 *   
 *   Params:
 *      file: Open file object.
 *      startFrom: Starting offset address.
 */
void readData (FILE *file, unsigned int startFrom)
{
    unsigned char b1[1], buffer[32];
    unsigned int readedByte, totalSize = 0;
    char headerFiledBuff[32];

    fseek (file, startFrom, SEEK_SET);

    // Read size.
    for (int i=0;i<2;i++)
    {
        fread (b1, sizeof (b1), 1, file); // Read & advance 1 byte.
            
        readedByte = b1[0];
        totalSize += i == 0 ? b1[0]<<8 : b1[0];
    }

    // Read type flag.
    for (int i=0;i<sizeof (buffer);i++)
    {
        fread (b1, sizeof (b1), 1, file); // Read & advance 1 byte.
            
        readedByte = b1[0];
        headerFiledBuff[i] = readedByte % 2 == 0 ? '0' : '1';
    }

    char binBuff[9], controlFlag[5];

    for (int i=0, n=0;i<sizeof (headerFiledBuff);i+=8, n++)
    {
        strncpy (binBuff, &headerFiledBuff[i], 8);

        binBuff[8] = '\0';
        controlFlag[n] = (char) strtol (binBuff, NULL, 2);
    }

    controlFlag[4] = '\0';

    if (!strcmp (controlFlag, "vstg"))
    {
        char messageBuff[totalSize - (3 + 32) + 1]; // (size + type + \0 char)

        // Read hidden message.
        for (int i=0;i<sizeof (messageBuff);i++)
        {
            fread (b1, sizeof (b1), 1, file); // Read & advance 1 byte.
                
            readedByte = b1[0];

            messageBuff[i] = readedByte % 2 == 0 ? '0' : '1';
        }

        char binMsgBuff[9];

        for (int i=0, n=0;i<sizeof (messageBuff);i+=8, n++)
        {
            strncpy (binMsgBuff, &messageBuff[i], 8);
            binMsgBuff[8] = '\0';
            printf ("%c", (char) strtol (binMsgBuff, NULL, 2));
        }
        printf ("\n");
    }
}

/*
 * Procedure: abracadabra
 * ----------------------------
 *   This is the procedure that does the magic. It's used both to write and to read the information hidden
 *   between the bytes in file.
 *   
 *   Params:
 *      file: Open file object.
 *      mode: Mode can be "write" (w) or "read" (r).
 *      blockOffset: Starting offset address.
 *      msgSize: Message size. Only used in "write" (w) mode.
 *      msg: Message itself. If the mode is "read" (r) the message will be empty and will not be used.
 */
void abracadabra (FILE *file, char mode, unsigned int blockOffset, unsigned int msgSize, char *msg)
{
    unsigned char buffer[4];
    unsigned int blockSize, type;

    fseek (file, blockOffset, SEEK_SET);

    fread (buffer, sizeof (buffer), 1, file); // Read & advance 4 bytes.
    blockSize = (buffer[0]<<24) | (buffer[1]<<16) | (buffer[2]<<8) | buffer[3];

    fread (buffer, sizeof (buffer), 1, file); // Read & advance 4 bytes.
    type = (buffer[0]<<24) | (buffer[1]<<16) | (buffer[2]<<8) | buffer[3];

    if (type == mdat && blockSize > 8)
    {
        unsigned int chunkSize = blockSize / 3;
        unsigned int startFrom = blockOffset + chunkSize;

        switch (mode)
        {
        case 'w':
            writeData (file, startFrom, msgSize, msg);
            break;
        case 'r':
            readData (file, startFrom);
            break;
        default:
            break;
        }
    }
}

/*
 * Procedure: processFile
 * ----------------------------
 *   This procedure will process the file in different ways like writing, reading or displaying information,
 *   it depends on the mode.
 *   
 *   Params:
 *      file: Open file object.
 *      mode: Mode can be "write" (w) or "read" (r).
 *      msg: Message itself. If the mode is "read" (r) the message will be empty and will not be used.
 */
void processFile (FILE *file, char mode, char *msg)
{
    unsigned int from = 0;
    unsigned int to = getFileSize (file);
    int numBlocks = getNumBlocks (file, from, to);

    unsigned int arr[numBlocks];
    unsigned int *blocks = getBlocksOffsets (file, arr, from, to);
    unsigned int msgSize = strlen (msg) * 8;

    switch (mode)
    {
    case 'w':
    case 'r':
        for (int i=0;i < numBlocks;i++)
        {
            abracadabra (file, mode, blocks[i], msgSize, msg);
        }
        break;
    case 'i':
        for (int i=0, currentBlockNumber=1;i < numBlocks;i++, currentBlockNumber++)
        {
            printBlockInfo (file, blocks[i], currentBlockNumber, numBlocks, 0);
        }
        break;
    default:
        break;
    }
}
