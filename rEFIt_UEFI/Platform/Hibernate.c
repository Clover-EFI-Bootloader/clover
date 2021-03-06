/*
 *  Hibernate.c
 *
 *  Created by dmazar, 01.2014.
 *
 *  Hibernate support.
 *
 */


#include "Platform.h"


#ifndef DEBUG_ALL
#define DEBUG_HIB 1
#else
#define DEBUG_HIB DEBUG_ALL
#endif

#if DEBUG_HIB == 0
#define DBG(...)
#else
#define DBG(...) DebugLog(DEBUG_HIB, __VA_ARGS__);
//#define DBG(...) AsciiPrint(__VA_ARGS__);
#endif

#pragma pack(push, 1)

//
// Just the first part of HFS+ volume header from where we can take modification time
//
typedef struct _HFSPlusVolumeHeaderMin {
    UINT16              signature;
    UINT16              version;
    UINT32              attributes;
    UINT32              lastMountedVersion;
    UINT32              journalInfoBlock;
    
    UINT32              createDate;
    UINT32              modifyDate;
    UINT32              backupDate;
    UINT32              checkedDate;
    
    UINT32              fileCount;
    UINT32              folderCount;
    
    UINT32              blockSize;
    UINT32              totalBlocks;
    UINT32              freeBlocks;
} HFSPlusVolumeHeaderMin;

// IOHibernateImageHeader.signature
enum
{
    kIOHibernateHeaderSignature        = 0x73696d65,
    kIOHibernateHeaderInvalidSignature = 0x7a7a7a7a
};

typedef struct _IOHibernateImageHeaderMin
{
    UINT64	imageSize;
    UINT64	image1Size;
    
    UINT32	restore1CodePhysPage;
    UINT32    reserved1;
    UINT64	  restore1CodeVirt;
    UINT32	restore1PageCount;
    UINT32	restore1CodeOffset;
    UINT32	restore1StackOffset;
    
    UINT32	pageCount;
    UINT32	bitmapSize;
    
    UINT32	restore1Sum;
    UINT32	image1Sum;
    UINT32	image2Sum;
    
    UINT32	actualRestore1Sum;
    UINT32	actualImage1Sum;
    UINT32	actualImage2Sum;
    
    UINT32	actualUncompressedPages;
    UINT32	conflictCount;
    UINT32	nextFree;
    
    UINT32	signature;
    UINT32	processorFlags;
  UINT32    runtimePages;
  UINT32    runtimePageCount;
  UINT64    runtimeVirtualPages;

  UINT32    performanceDataStart;
  UINT32    performanceDataSize;

  UINT64	encryptStart;
  UINT64	machineSignature;

  UINT32    previewSize;
  UINT32    previewPageListSize;

  UINT32	diag[4];

  UINT32    handoffPages;
  UINT32    handoffPageCount;

  UINT32    systemTableOffset;

  UINT32	debugFlags;
  UINT32	options;
  UINT32	sleepTime;
  UINT32    compression;

} IOHibernateImageHeaderMin;

typedef struct _IOHibernateImageHeaderMinSnow
{
  UINT64	imageSize;
  UINT64	image1Size;

  UINT32	restore1CodePhysPage;
  UINT32	restore1PageCount;
  UINT32	restore1CodeOffset;
  UINT32	restore1StackOffset;

  UINT32	pageCount;
  UINT32	bitmapSize;

  UINT32	restore1Sum;
  UINT32	image1Sum;
  UINT32	image2Sum;

  UINT32	actualRestore1Sum;
  UINT32	actualImage1Sum;
  UINT32	actualImage2Sum;

  UINT32	actualUncompressedPages;
  UINT32	conflictCount;
  UINT32	nextFree;

  UINT32	signature;
  UINT32	processorFlags;
} IOHibernateImageHeaderMinSnow;


typedef struct _AppleRTCHibernateVars
{
    UINT8     signature[4];
    UINT32    revision;
    UINT8	  booterSignature[20];
    UINT8	  wiredCryptKey[16];
} AppleRTCHibernateVars;

#pragma pack(pop)

//
// Taken from VBoxFsDxe
//

//
// time conversion
//
// Adopted from public domain code in FreeBSD libc.
//

#define SECSPERMIN      60
#define MINSPERHOUR     60
#define HOURSPERDAY     24
#define DAYSPERWEEK     7
#define DAYSPERNYEAR    365
#define DAYSPERLYEAR    366
#define SECSPERHOUR     (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY      ((INTN) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR     12

#define EPOCH_YEAR      1970
#define EPOCH_WDAY      TM_THURSDAY

#define isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))
#define LEAPS_THRU_END_OF(y)    ((y) / 4 - (y) / 100 + (y) / 400)

INT32 mon_lengths[2][MONSPERYEAR] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};
INT32 year_lengths[2] = {
    DAYSPERNYEAR, DAYSPERLYEAR
};

//static fsw_u32 mac_to_posix(fsw_u32 mac_time)
INT32 mac_to_posix(UINT32 mac_time)
{
    /* Mac time is 1904 year based */
    return mac_time ?  mac_time - 2082844800 : 0;
}
/* not used
VOID fsw_efi_decode_time(OUT EFI_TIME *EfiTime, IN UINT32 UnixTime)
{
    INT32        days, rem;
    INT32        y, newy, yleap;
    INT32        *ip;
    
    ZeroMem(EfiTime, sizeof(EFI_TIME));
    
    days = ((INTN)UnixTime) / SECSPERDAY;
    rem = ((INTN)UnixTime) % SECSPERDAY;
    
    EfiTime->Hour = (UINT8) (rem / SECSPERHOUR);
    rem = rem % SECSPERHOUR;
    EfiTime->Minute = (UINT8) (rem / SECSPERMIN);
    EfiTime->Second = (UINT8) (rem % SECSPERMIN);
    
    y = EPOCH_YEAR;
    while (days < 0 || days >= (INT64) year_lengths[yleap = isleap(y)]) {
        newy = y + days / DAYSPERNYEAR;
        if (days < 0)
            --newy;
        days -= (newy - y) * DAYSPERNYEAR +
        LEAPS_THRU_END_OF(newy - 1) -
        LEAPS_THRU_END_OF(y - 1);
        y = newy;
    }
    EfiTime->Year = (UINT16)y;
    ip = mon_lengths[yleap];
    for (EfiTime->Month = 0; days >= (INT64) ip[EfiTime->Month]; ++(EfiTime->Month))
        days = days - (INT64) ip[EfiTime->Month];
    EfiTime->Month++;  // adjust range to EFI conventions
    EfiTime->Day = (UINT8) (days + 1);
}
*/


EFI_BLOCK_READ OrigBlockIoRead = NULL;
UINT64  gSleepImageOffset = 0;
UINT32  gSleepTime = 0;


/** BlockIo->Read() override. */
EFI_STATUS
EFIAPI OurBlockIoRead (
                       IN EFI_BLOCK_IO_PROTOCOL          *This,
                       IN UINT32                         MediaId,
                       IN EFI_LBA                        Lba,
                       IN UINTN                          BufferSize,
                       OUT VOID                          *Buffer
                       )
{
  EFI_STATUS          Status;
  Status = OrigBlockIoRead(This, MediaId, Lba, BufferSize, Buffer);
  
  // Enter special processing only when gSleepImageOffset == 0, to avoid recursion when Boot/Log=true
  if (gSleepImageOffset == 0 && Status == EFI_SUCCESS && BufferSize >= sizeof(IOHibernateImageHeaderMin)) { //sizeof(IOHibernateImageHeaderMin)==96
    IOHibernateImageHeaderMin *Header;
    IOHibernateImageHeaderMinSnow *Header2;
    UINT32 BlockSize = 0;
    
    // Mark that we are executing, to avoid entering above phrase again, and don't add DBGs outside this scope, to avoid recursion
    gSleepImageOffset = (UINT64)-1;
    
    if (This->Media != NULL) {
      BlockSize = This->Media->BlockSize;
    } else {
      BlockSize = 512;
    }
    
    DBG(" OurBlockIoRead: Lba=%lx, Offset=%lx (BlockSize=%d)\n", Lba, MultU64x32(Lba, BlockSize), BlockSize);
    
    Header = (IOHibernateImageHeaderMin *) Buffer;
    Header2 = (IOHibernateImageHeaderMinSnow *) Buffer;
    DBG(" sig lion: %x\n", Header->signature);
    DBG(" sig snow: %x\n", Header2->signature);
    // DBG(" sig swap: %x\n", SwapBytes32(Header->signature));
    
    if (Header->signature == kIOHibernateHeaderSignature ||
        Header2->signature == kIOHibernateHeaderSignature) {
      gSleepImageOffset = MultU64x32(Lba, BlockSize);
      DBG(" got sleep image offset\n");
      //save sleep time as lvs1974 suggested
      if (Header->signature == kIOHibernateHeaderSignature) {
        gSleepTime = Header->sleepTime;
      } else
        gSleepTime = 0;
      // return invalid parameter in case of success in order to prevent driver from caching our buffer
      return EFI_INVALID_PARAMETER;
    } else {
      DBG(" no valid sleep image offset was found\n");
      gSleepImageOffset = 0;
    }
  }
  
  return Status;
}

/** Get slep image location (volume and name) */
VOID
GetSleepImageLocation(IN REFIT_VOLUME *Volume, REFIT_VOLUME **SleepImageVolume, CHAR16 **SleepImageName)
{
  EFI_STATUS          Status;
  UINT8               *PrefBuffer = NULL;
  UINTN               PrefBufferLen = 0;
  TagPtr              PrefDict, dict, dict2, prop;
  CHAR16              *PrefName = L"\\Library\\Preferences\\SystemConfiguration\\com.apple.PowerManagement.plist";
  CHAR16              *ImageName = NULL;
  REFIT_VOLUME        *ImageVolume = Volume;
  
  // find sleep image entry from plist
  Status = egLoadFile(Volume->RootDir, PrefName, &PrefBuffer, &PrefBufferLen);
  DBG("    read prefs %s status=%r\n", PrefName, Status);
  if (!EFI_ERROR(Status)) {
    Status = ParseXML((const CHAR8*)PrefBuffer, &PrefDict, 0);
    if (!EFI_ERROR(Status)) {
      dict = GetProperty(PrefDict, "Custom Profile");
      if (dict) {
        dict2 = GetProperty(dict, "AC Power");
        if (dict2) {
          prop = GetProperty(dict2, "Hibernate File");
          if (prop && prop->type == kTagTypeString ) {
            CHAR16 *p;
            if (AsciiStrStr(prop->string, "/Volumes/")) {
              CHAR8 *VolNameStart = NULL, *VolNameEnd = NULL;
              CHAR16 *VolName = NULL;
              UINTN VolNameSize = 0;
              // Extract Volumes Name
              VolNameStart = AsciiStrStr(prop->string + 1, "/") + 1;
              if (VolNameStart) {
                VolNameEnd = AsciiStrStr(VolNameStart, "/");
                if (VolNameEnd) {
                  VolNameSize = (VolNameEnd - VolNameStart + 1) * sizeof(CHAR16);
                  if (VolNameSize > 0) {
                    VolName = AllocateZeroPool(VolNameSize);
                  }
                }
              }
              if (VolName) {
                UnicodeSPrint(VolName, VolNameSize, L"%a", VolNameStart);
                ImageVolume = FindVolumeByName(VolName);
                if (ImageVolume) {
                  ImageName = PoolPrint(L"%a", VolNameEnd);
                } else {
                  ImageVolume = Volume;
                }
                FreePool(VolName);
              }
            } else if (AsciiStrStr(prop->string, "/var") && !AsciiStrStr(prop->string, "private")) {
              ImageName = PoolPrint(L"\\private%a", prop->string);
            } else {
              ImageName = PoolPrint(L"%a", prop->string);
            }
            p = ImageName;
            while (*p) {
              if (*p == L'/') {
                *p = L'\\';
              }
              p++;
            }
            DBG("    SleepImage name from pref: ImageVolume = '%s', ImageName = '%s'\n", ImageVolume->VolName, ImageName);
          }
        }
      }
    }
  }
  
  if (!ImageName) {
    ImageName = PoolPrint(L"\\private\\var\\vm\\sleepimage");
    DBG("    using default sleep image name = %s\n", ImageName);
  }
  if (PrefBuffer) {
    FreePool(PrefBuffer); //allocated by egLoadFile
  }
  
  *SleepImageVolume = ImageVolume;
  *SleepImageName = ImageName;
}


/** Returns byte offset of sleepimage on the whole disk or 0 if not found or error.
 *
 * To avoid messing with HFS+ format, we'll use the trick with overriding
 * BlockIo->Read() of the disk and then read first bytes of the sleepimage
 * through file system driver. And then we'll detect block delivered by BlockIo
 * and calculate position from there.
 * It's for hack after all :)
 */
UINT64
GetSleepImagePosition (IN REFIT_VOLUME *Volume, REFIT_VOLUME **SleepImageVolume)
{
  EFI_STATUS          Status;
  EFI_FILE            *File;
  VOID                *Buffer;
  UINTN               BufferSize;
  CHAR16              *ImageName;
  REFIT_VOLUME        *ImageVolume;
  
  if (!Volume) {
    DBG("    no volume to get sleepimage\n");
    return 0;
  }

  if (Volume->WholeDiskBlockIO == NULL) {
    DBG("    no disk BlockIo\n");
    return 0;
  }

  // If IsSleepImageValidBySignature() was used, then we already have that offset
  if (Volume->SleepImageOffset != 0) {
    if (SleepImageVolume != NULL) {
      // Update caller's SleepImageVolume when requested
      GetSleepImageLocation(Volume,SleepImageVolume,&ImageName);
    }
    DBG("    returning previously calculated offset: %lx\n", Volume->SleepImageOffset);
    return Volume->SleepImageOffset;
  }
  
  // Get sleepimage name and volume
  GetSleepImageLocation(Volume,&ImageVolume,&ImageName);

  // Open sleepimage
  Status = ImageVolume->RootDir->Open(ImageVolume->RootDir, &File, ImageName, EFI_FILE_MODE_READ, 0);
  if (EFI_ERROR(Status)) {
    DBG("    sleepimage not found -> %r\n", Status);
    return 0;
  }

  // We want to read the first 512 bytes from sleepimage
  BufferSize = 512;
  Buffer = AllocatePool(BufferSize);
  if (Buffer == NULL) {
    DBG("    could not allocate buffer for sleepimage\n");
    return 0;
  }

  DBG("    Reading first %d bytes of sleepimage ...\n", BufferSize);

  // Override disk BlockIo
  OrigBlockIoRead = ImageVolume->WholeDiskBlockIO->ReadBlocks;
  ImageVolume->WholeDiskBlockIO->ReadBlocks = OurBlockIoRead;
  gSleepImageOffset = 0; //used as temporary global variable to pass our value
  Status = File->Read(File, &BufferSize, Buffer);

  // Restore original disk BlockIo
  ImageVolume->WholeDiskBlockIO->ReadBlocks = OrigBlockIoRead;

  // OurBlockIoRead always returns invalid parameter in order to avoid driver caching, so that is a good value
  if (Status == EFI_INVALID_PARAMETER) {
    Status = EFI_SUCCESS;
  }
  DBG("    Reading completed -> %r\n", Status);

  // Close sleepimage
  File->Close(File);
  
  // We don't use the buffer, as actual signature checking is being done by OurBlockIoRead
  if (Buffer) {
    FreePool(Buffer);
  }

  if (EFI_ERROR(Status)) {
    DBG("     can not read sleepimage -> %r\n", Status);
    return 0;
  } 

  // We store SleepImageOffset, in case our BlockIoRead does not execute again on next read due to driver caching.
  if (gSleepImageOffset != 0) {
    DBG("     sleepimage offset acquired successfully: %lx\n", gSleepImageOffset);
    ImageVolume->SleepImageOffset = gSleepImageOffset;
  } else {
    DBG("     sleepimage offset could not be acquired\n");
  }

  if (SleepImageVolume != NULL) {
    // Update caller's SleepImageVolume when requested
    *SleepImageVolume = ImageVolume;
  }
  return gSleepImageOffset;
}


/** Returns TRUE if /private/var/vm/sleepimage exists
 *  and it's modification time is close to volume modification time).
 */
BOOLEAN
IsSleepImageValidBySleepTime (IN REFIT_VOLUME *Volume)
{
  EFI_STATUS          Status;
  VOID                *Buffer;
  EFI_BLOCK_IO_PROTOCOL   *BlockIo;
  HFSPlusVolumeHeaderMin  *HFSHeader;
  UINT32              HFSVolumeModifyDate;
  INTN                TimeDiff;
  INTN                Pages; // = 1;
  //EFI_TIME            ImageModifyTime;
  //EFI_TIME            *TimePtr;
  //EFI_TIME            HFSVolumeModifyTime;

  DBG("     gSleepTime: %d\n", gSleepTime);
  //fsw_efi_decode_time(&ImageModifyTime, gSleepTime);
  //TimePtr = &ImageModifyTime;
  //DBG(" in EFI: %d-%d-%d %d:%d:%d\n", TimePtr->Year, TimePtr->Month, TimePtr->Day, TimePtr->Hour, TimePtr->Minute, TimePtr->Second);

  //
  // Get HFS+ volume nodification time
  //
  // use 4KB aligned page to avoid possible issues with BlockIo buffer alignment
  BlockIo = Volume->BlockIO;
  Pages = EFI_SIZE_TO_PAGES(BlockIo->Media->BlockSize);
  Buffer = AllocatePages(Pages);
  if (Buffer == NULL) {
    return FALSE;
  }
  Status = BlockIo->ReadBlocks(BlockIo, BlockIo->Media->MediaId, 2, BlockIo->Media->BlockSize, Buffer);
  if (EFI_ERROR(Status)) {
    DBG("     can not read HFS+ header -> %r\n", Status);
    FreePages(Buffer, Pages);
    return FALSE;
  }
  HFSHeader = (HFSPlusVolumeHeaderMin *)Buffer;
  HFSVolumeModifyDate = SwapBytes32(HFSHeader->modifyDate);
  HFSVolumeModifyDate = mac_to_posix(HFSVolumeModifyDate);
  DBG("     HFS+ volume modifyDate: %d\n", HFSVolumeModifyDate);
  //fsw_efi_decode_time(&HFSVolumeModifyTime, mac_to_posix(HFSVolumeModifyDate));
  //TimePtr = &HFSVolumeModifyTime;
  //DBG(" in EFI: %d-%d-%d %d:%d:%d\n", TimePtr->Year, TimePtr->Month, TimePtr->Day, TimePtr->Hour, TimePtr->Minute, TimePtr->Second);
  FreePages(Buffer, Pages);

  //
  // Check that sleepimage is not more then 5 secs older then volume modification date
  // Idea is from Chameleon
  //
  TimeDiff = HFSVolumeModifyDate - (INTN)gSleepTime;
  DBG("     image older then volume: %d sec\n", TimeDiff);
  if (TimeDiff > 5 /*|| TimeDiff < -5 */) {
    //Slice - if image newer then volume it should be OK
    DBG("     image too old\n");
    return FALSE;
  }

  return TRUE;
}

/** Returns TRUE if /private/var/vm/sleepimage exists
 *  and it's signature is kIOHibernateHeaderSignature.
 */
BOOLEAN
IsSleepImageValidBySignature (IN REFIT_VOLUME *Volume)
{
  // We'll have to detect offset here also in case driver caches
  // some data and stops us from detecting offset later.
  // So, make first call to GetSleepImagePosition() now.
  DBG("    Check sleep image 'by signature':\n");
  return (GetSleepImagePosition (Volume, NULL) != 0);
}



/** Returns TRUE if given OSX on given volume is hibernated. */
BOOLEAN
IsOsxHibernated (IN REFIT_VOLUME *Volume)
{
//  BOOLEAN IsHibernate = FALSE;
  EFI_STATUS          Status;
  UINTN               Size                = 0;
  UINT8               *Data               = NULL;

  DBG("    Check if volume Is Hibernated:\n");

  // CloverEFI or UEFI with EmuVariable
  if (IsSleepImageValidBySignature(Volume)) {
    if ((gSleepTime == 0) || IsSleepImageValidBySleepTime(Volume)) {
      DBG("     hibernated: yes\n");
    } else {
      DBG("     hibernated: no - time\n");
      return FALSE;
    }
//    IsHibernate = TRUE;
  } else {
    DBG("     hibernated: no - sign\n");
    return FALSE;
  }
  
  //if sleep image is good but OSX was not hibernated.
  //or we choose "cancel hibernate wake" then it must be canceled
  if (GlobalConfig.NeverHibernate) {
    DBG("     hibernated: set as never\n");
    return FALSE;
  }
  
  if (!gFirmwareClover &&
      !gDriversFlags.EmuVariableLoaded) {
    DBG("     UEFI with NVRAM: ");
    Status = gRT->GetVariable (L"Boot0082", &gEfiGlobalVariableGuid, NULL, &Size, Data);
    if (Status == EFI_BUFFER_TOO_SMALL) {
      DBG("yes\n");
//      return TRUE;
    } else {
      DBG("Boot0082 no\n");
      return FALSE;
    }
  }
  return TRUE;
}



/** Prepares nvram vars needed for boot.efi to wake from hibernation:
 *  boot-switch-vars and boot-image.
 *
 * Normally those vars should be set by kernel
 * boot-switch-vars: structure with image encription key
 * boot-image: device path like Acpi(PNP0A03,0)/Pci(1f,2)/Sata(2,0)/File(56b99e000)
 *  where Acpi(PNP0A03,0)/Pci(1f,2)/Sata(2,0) points to the disk containing sleepimage
 *  and File(56b99e000) contains hex position (in bytes) of the beginning of the sleepimage
 *
 * Since boot-switch-vars is not present in CloverEFI or with EmuVar driver (no real NVRAM) but also not on UEFI hack
 * (not written by the kernel for some reason), and since boot-image is also not present in CloverEFI
 * and on UEFI hack device path as set by kernel can be different in some bytes from the device path
 * reported by UEFI, we'll compute and set both vars here.
 *
 * That's the only way for CloverEFI and should be OK for UEFI hack also.
 */

BOOLEAN
PrepareHibernation (IN REFIT_VOLUME *Volume)
{
  EFI_STATUS          Status;
  UINT64          SleepImageOffset;
  CHAR16          OffsetHexStr[17];
  EFI_DEVICE_PATH_PROTOCOL    *BootImageDevPath;
  UINTN           Size;
  VOID            *Value;
  AppleRTCHibernateVars RtcVars;
  UINT8           *VarData = NULL;
  REFIT_VOLUME    *SleepImageVolume;
  
  DBG("PrepareHibernation:\n");
  
  // Find sleep image offset
  SleepImageOffset = GetSleepImagePosition (Volume,&SleepImageVolume);
  DBG(" SleepImageOffset: %lx\n", SleepImageOffset);
  if (SleepImageOffset == 0 || SleepImageVolume == NULL) {
    DBG(" sleepimage offset not found\n");
    return FALSE;
  }
  
  // Set boot-image var
  UnicodeSPrint(OffsetHexStr, sizeof(OffsetHexStr), L"%lx", SleepImageOffset);
  BootImageDevPath = FileDevicePath(SleepImageVolume->WholeDiskDeviceHandle, OffsetHexStr);
//  DBG(" boot-image device path:\n");
  Size = GetDevicePathSize(BootImageDevPath);
  VarData = (UINT8*)BootImageDevPath;
  PrintBytes(VarData, Size);
  DBG("boot-image before: %s\n", FileDevicePathToStr(BootImageDevPath));
  //      VarData[6] = 8;
  
//  VarData[24] = 0xFF;
//  VarData[25] = 0xFF;
//  DBG("boot-image corrected: %s\n", FileDevicePathToStr(BootImageDevPath));
  
  Status = gRT->SetVariable(L"boot-image", &gEfiAppleBootGuid,
                            EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                            Size , BootImageDevPath);
  if (EFI_ERROR(Status)) {
    DBG(" can not write boot-image -> %r\n", Status);
    return FALSE;
  }

  // now we should delete boot0082 to do hibernate only once
  Status = DeleteBootOption(0x82);
  if (EFI_ERROR(Status)) {
    DBG("Options 0082 was not deleted: %r\n", Status);
  }

  // if boot-switch-vars exists (NVRAM working), then use it
  Status = GetVariable2 (L"boot-switch-vars", &gEfiAppleBootGuid, &Value, &Size);
  if (!EFI_ERROR(Status)) {
    // leave it as is
    DBG(" boot-switch-vars present\n");
    return TRUE;
  }
  
  // if IOHibernateRTCVariables exists (NVRAM working), then copy it to boot-switch-vars
  // else (no NVRAM) set boot-switch-vars to dummy one
  Status = GetVariable2 (L"IOHibernateRTCVariables", &gEfiAppleBootGuid, &Value, &Size);
  if (!EFI_ERROR(Status)) {
    DBG(" IOHibernateRTCVariables found - will be used as boot-switch-vars\n");
    // delete IOHibernateRTCVariables
    Status = gRT->SetVariable(L"IOHibernateRTCVariables", &gEfiAppleBootGuid,
                              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                              0, NULL);
  } else {
    // no NVRAM
    DBG(" setting dummy boot-switch-vars\n");
    Size = sizeof(RtcVars);
    Value = &RtcVars;
    SetMem(&RtcVars, Size, 0);
    RtcVars.signature[0] = 'A';
    RtcVars.signature[1] = 'A';
    RtcVars.signature[2] = 'P';
    RtcVars.signature[3] = 'L';
    RtcVars.revision     = 1;
  }
  
  Status = gRT->SetVariable(L"boot-switch-vars", &gEfiAppleBootGuid,
                            EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                            Size, Value);
  if (EFI_ERROR(Status)) {
    DBG(" can not write boot-switch-vars -> %r\n", Status);
    return FALSE;
  }
  
  return TRUE;
}

