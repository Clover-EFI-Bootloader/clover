/*
 * refind/apple.c
 * Functions specific to Apple computers
 * 
 * Copyright (c) 2015 Roderick W. Smith
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "entry_scan.h"
#include "lib.h"
#include "apple.h"

/*
 * The below definitions and SetAppleOSInfo() function are based on a GRUB patch
 * by Andreas Heider:
 * https://lists.gnu.org/archive/html/grub-devel/2013-12/msg00442.html
 */

#define EFI_APPLE_SET_OS_PROTOCOL_GUID  \
  { 0xc5c5da95, 0x7d5c, 0x45e6, \
    { 0xb2, 0xf1, 0x3f, 0xd5, 0x2b, 0xb1, 0x00, 0x77 } \
  }

typedef struct EfiAppleSetOsInterface {
    UINT64 Version;
    EFI_STATUS EFIAPI (*SetOsVersion) (IN CHAR8 *Version);
    EFI_STATUS EFIAPI (*SetOsVendor) (IN CHAR8 *Vendor);
} EfiAppleSetOsInterface;

// Function to tell the firmware that OS X is being launched. This is
// required to work around problems on some Macs that don't fully
// initialize some hardware (especially video displays) when third-party
// OSes are launched in EFI mode.
EFI_STATUS SetAppleOSInfo() {
    CHAR16 *AppleOSVersion = NULL;
    CHAR8 *AppleOSVersion8 = NULL;
    EFI_STATUS Status;
    EFI_GUID apple_set_os_guid = EFI_APPLE_SET_OS_PROTOCOL_GUID;
    EfiAppleSetOsInterface *SetOs = NULL;

    Status = gBS->LocateProtocol (&apple_set_os_guid, NULL, (VOID**) &SetOs);

    // If not a Mac, ignore the call....
    if ((Status != EFI_SUCCESS) || (!SetOs))
        return EFI_SUCCESS;

    if ((SetOs->Version != 0) && gSettings.SpoofOSXVersion) {
        AppleOSVersion = PoolPrint (L"Mac OS X %s", gSettings.SpoofOSXVersion);
        if (AppleOSVersion) {
            AppleOSVersion8 = AllocateZeroPool((StrLen(AppleOSVersion) + 1) * sizeof(CHAR8));
            UnicodeStrToAsciiStr(AppleOSVersion, AppleOSVersion8);
            if (AppleOSVersion8) {
                Status = SetOs->SetOsVersion (AppleOSVersion8);
                if (!EFI_ERROR(Status))
                    Status = EFI_SUCCESS;
                FreePool(AppleOSVersion8);
            } else {
                Status = EFI_OUT_OF_RESOURCES;
                Print(L"Out of resources in SetAppleOSInfo!\n");
            }
            if ((Status == EFI_SUCCESS) && (SetOs->Version >= 2))
                Status = SetOs->SetOsVendor ((CHAR8 *) "Apple Inc.");
            FreePool(AppleOSVersion);
        } // if (AppleOSVersion)
    } // if
    if (Status != EFI_SUCCESS)
        Print(L"Unable to set firmware boot type!\n");

    return (Status);
} // EFI_STATUS SetAppleOSInfo()
