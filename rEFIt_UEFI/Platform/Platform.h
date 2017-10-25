/*
Headers collection for procedures
*/

#ifndef __REFIT_PLATFORM_H__
#define __REFIT_PLATFORM_H__

// Set all debug options - apianti
// Uncomment to set all debug options
// Comment to use source debug options
//#define DEBUG_ALL 2


#include <Uefi.h>

#include <Guid/Acpi.h>
#include <Guid/EventGroup.h>
#include <Guid/SmBios.h>
#include <Guid/Mps.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/GenericBdsLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PerformanceLib.h>
#include <Library/PeCoffGetEntryPointLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UsbMass.h>
#include <Library/VideoBiosPatchLib.h>
#include <Library/MemLogLib.h>

#include <Framework/FrameworkInternalFormRepresentation.h>

#include <IndustryStandard/Acpi10.h>
#include <IndustryStandard/Acpi20.h>
#include <IndustryStandard/Atapi.h>
#include <IndustryStandard/SmbiosA.h>
#include <IndustryStandard/Bmp.h>

#include <Protocol/Cpu.h>
#include <Protocol/CpuIo.h>
#include <Protocol/DataHub.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/EdidOverride.h>
#include <Protocol/FrameworkHii.h>
#include <Protocol/SimplePointer.h>
#include <Protocol/Smbios.h>
#include <Protocol/VariableWrite.h>
#include <Protocol/Variable.h>

#include <Protocol/FSInjectProtocol.h>
#include <Protocol/MsgLog.h>
#include <Protocol/efiConsoleControl.h>
#include <Protocol/EmuVariableControl.h>

#include "../refit/lib.h"
#include "string.h"
#include "boot.h"
//#include "PiBootMode.h"
#include "../refit/IO.h"
#include "device_inject.h"
#include "kext_inject.h"
//#include "entry_scan.h"

#define CLOVER_SIGN        SIGNATURE_32('C','l','v','r')

/* XML Tags */
#define kXMLTagPList     "plist"
#define kXMLTagDict      "dict"
#define kXMLTagKey       "key"
#define kXMLTagString    "string"
#define kXMLTagInteger   "integer"
#define kXMLTagData      "data"
#define kXMLTagDate      "date"
#define kXMLTagFalse     "false/"
#define kXMLTagTrue      "true/"
#define kXMLTagArray     "array"
#define kXMLTagReference "reference"
#define kXMLTagID        "ID="
#define kXMLTagIDREF     "IDREF="

#define MAX_NUM_DEVICES  64

/* Decimal powers: */
#define kilo (1000ULL)
#define Mega (kilo * kilo)
#define Giga (kilo * Mega)
#define Tera (kilo * Giga)
#define Peta (kilo * Tera)

#define EBDA_BASE_ADDRESS            0x40E
#define EFI_SYSTEM_TABLE_MAX_ADDRESS 0xFFFFFFFF
#define ROUND_PAGE(x)                ((((unsigned)(x)) + EFI_PAGE_SIZE - 1) & ~(EFI_PAGE_SIZE - 1))

//
// Max bytes needed to represent ID of a SCSI device
//
#define EFI_SCSI_TARGET_MAX_BYTES (0x10)

//
// bit5..7 are for Logical unit number
// 11100000b (0xe0)
//
#define EFI_SCSI_LOGICAL_UNIT_NUMBER_MASK 0xe0

//
// Scsi Command Length
//
#define EFI_SCSI_OP_LENGTH_SIX      0x6
#define EFI_SCSI_OP_LENGTH_TEN      0xa
#define EFI_SCSI_OP_LENGTH_SIXTEEN  0x10

//#define SAFE_LOG_SIZE	80

#define MSG_LOG_SIZE (256 * 1024)
#define PREBOOT_LOG  L"EFI\\CLOVER\\misc\\preboot.log"
#define LEGBOOT_LOG  L"EFI\\CLOVER\\misc\\legacy_boot.log"
#define BOOT_LOG     L"EFI\\CLOVER\\misc\\boot.log"
#define SYSTEM_LOG   L"EFI\\CLOVER\\misc\\system.log"
#define DEBUG_LOG    L"EFI\\CLOVER\\misc\\debug.log"
#define PREWAKE_LOG  L"EFI\\CLOVER\\misc\\prewake.log"
//#define MsgLog(x...) {AsciiSPrint(msgCursor, MSG_LOG_SIZE, x); while(*msgCursor){msgCursor++;}}
//#define MsgLog(...)  {AsciiSPrint(msgCursor, (MSG_LOG_SIZE-(msgCursor-msgbuf)), __VA_ARGS__); while(*msgCursor){msgCursor++;}}
#ifndef DEBUG_ALL
#define MsgLog(...)  DebugLog(1, __VA_ARGS__)
#else
#define MsgLog(...)  DebugLog(DEBUG_ALL, __VA_ARGS__)
#endif

#define CPU_MODEL_PENTIUM_M     0x09
#define CPU_MODEL_DOTHAN        0x0D
#define CPU_MODEL_YONAH         0x0E
#define CPU_MODEL_MEROM         0x0F  /* same as CONROE but mobile */
#define CPU_MODEL_CONROE        0x0F  /* Allendale, Conroe, Kentsfield, Woodcrest, Clovertown, Tigerton */
#define CPU_MODEL_CELERON       0x16  /* ever see? */
#define CPU_MODEL_PENRYN        0x17  /* Yorkfield, Harpertown, Penryn M */
#define CPU_MODEL_WOLFDALE      0x17  /* kind of penryn but desktop */
#define CPU_MODEL_NEHALEM       0x1A  /* Bloomfield. Nehalem-EP, Nehalem-WS, Gainestown */
#define CPU_MODEL_ATOM          0x1C  /* Pineview UN */
#define CPU_MODEL_XEON_MP       0x1D  /* MP 7400 UN */
#define CPU_MODEL_FIELDS        0x1E  /* Lynnfield, Clarksfield, Jasper */
#define CPU_MODEL_DALES         0x1F  /* Havendale, Auburndale */
#define CPU_MODEL_CLARKDALE     0x25  /* Clarkdale, Arrandale */
#define CPU_MODEL_ATOM_SAN      0x26  /* Haswell H ? */
#define CPU_MODEL_LINCROFT      0x27  /* UN */
#define CPU_MODEL_SANDY_BRIDGE	0x2A
#define CPU_MODEL_WESTMERE      0x2C  /* Gulftown LGA1366 */
#define CPU_MODEL_JAKETOWN      0x2D  /* Sandy Bridge Xeon LGA2011 */
#define CPU_MODEL_NEHALEM_EX    0x2E
#define CPU_MODEL_WESTMERE_EX   0x2F
#define CPU_MODEL_ATOM_2000     0x36  /* UN */
#define CPU_MODEL_ATOM_3700     0x37  /* Bay Trail */
#define CPU_MODEL_IVY_BRIDGE    0x3A
#define CPU_MODEL_HASWELL       0x3C  /* Haswell DT */
#define CPU_MODEL_HASWELL_U5    0x3D  /* Haswell U5  5th generation Broadwell*/
#define CPU_MODEL_IVY_BRIDGE_E5 0x3E  /* Ivy Bridge Xeon UN */
#define CPU_MODEL_HASWELL_E     0x3F	/* Haswell Extreme */
//#define CPU_MODEL_HASWELL_H		0x??	// Haswell H
#define CPU_MODEL_HASWELL_ULT		0x45	/* Haswell ULT */
#define CPU_MODEL_CRYSTALWELL		0x46	/* Haswell ULX CPUID_MODEL_CRYSTALWELL */
#define CPU_MODEL_BROADWELL_HQ  0x47
#define CPU_MODEL_AIRMONT       0x4C
#define CPU_MODEL_AVOTON        0x4D
#define CPU_MODEL_SKYLAKE_U     0x4E
#define CPU_MODEL_BROADWELL_DE  0x56
#define CPU_MODEL_KNIGHT        0x57
#define CPU_MODEL_MOOREFIELD    0x5A
#define CPU_MODEL_GOLDMONT      0x5C
#define CPU_MODEL_ATOM_X3       0x5D
#define CPU_MODEL_SKYLAKE_S     0x5E
#define CPU_MODEL_CANNONLAKE    0x66

#define CPU_VENDOR_INTEL        0x756E6547
#define CPU_VENDOR_AMD          0x68747541
/* Unknown CPU */
#define CPU_STRING_UNKNOWN	    "Unknown CPU Type"

//definitions from Apple XNU

/* CPU defines */
#define bit(n)		            	(1UL << (n))
#define _Bit(n)			            (1ULL << (n))
#define _HBit(n)		            (1ULL << ((n)+32))

#define bitmask(h,l)	          ((bit(h)|(bit(h)-1)) & ~(bit(l)-1))
#define bitfield(x,h,l)	        RShiftU64(((x) & bitmask((h),(l))), (l))
#define quad(hi,lo)             ((LShiftU64((hi), 32) | (lo)))

/*
 * The CPUID_FEATURE_XXX values define 64-bit values
 * returned in %ecx:%edx to a CPUID request with %eax of 1: 
 */
#define	CPUID_FEATURE_FPU       _Bit(0)	/* Floating point unit on-chip */
#define	CPUID_FEATURE_VME       _Bit(1)	/* Virtual Mode Extension */
#define	CPUID_FEATURE_DE        _Bit(2)	/* Debugging Extension */
#define	CPUID_FEATURE_PSE       _Bit(3)	/* Page Size Extension */
#define	CPUID_FEATURE_TSC       _Bit(4)	/* Time Stamp Counter */
#define	CPUID_FEATURE_MSR       _Bit(5)	/* Model Specific Registers */
#define CPUID_FEATURE_PAE       _Bit(6)	/* Physical Address Extension */
#define	CPUID_FEATURE_MCE       _Bit(7)	/* Machine Check Exception */
#define	CPUID_FEATURE_CX8       _Bit(8)	/* CMPXCHG8B */
#define	CPUID_FEATURE_APIC      _Bit(9)	/* On-chip APIC */
#define CPUID_FEATURE_SEP       _Bit(11)	/* Fast System Call */
#define	CPUID_FEATURE_MTRR      _Bit(12)	/* Memory Type Range Register */
#define	CPUID_FEATURE_PGE       _Bit(13)	/* Page Global Enable */
#define	CPUID_FEATURE_MCA       _Bit(14)	/* Machine Check Architecture */
#define	CPUID_FEATURE_CMOV      _Bit(15)	/* Conditional Move Instruction */
#define CPUID_FEATURE_PAT       _Bit(16)	/* Page Attribute Table */
#define CPUID_FEATURE_PSE36     _Bit(17)	/* 36-bit Page Size Extension */
#define CPUID_FEATURE_PSN       _Bit(18)	/* Processor Serial Number */
#define CPUID_FEATURE_CLFSH     _Bit(19)	/* CLFLUSH Instruction supported */
#define CPUID_FEATURE_DS        _Bit(21)	/* Debug Store */
#define CPUID_FEATURE_ACPI      _Bit(22)	/* Thermal monitor and Clock Ctrl */
#define CPUID_FEATURE_MMX       _Bit(23)	/* MMX supported */
#define CPUID_FEATURE_FXSR      _Bit(24)	/* Fast floating pt save/restore */
#define CPUID_FEATURE_SSE       _Bit(25)	/* Streaming SIMD extensions */
#define CPUID_FEATURE_SSE2      _Bit(26)	/* Streaming SIMD extensions 2 */
#define CPUID_FEATURE_SS        _Bit(27)	/* Self-Snoop */
#define CPUID_FEATURE_HTT       _Bit(28)	/* Hyper-Threading Technology */
#define CPUID_FEATURE_TM        _Bit(29)	/* Thermal Monitor (TM1) */
#define CPUID_FEATURE_PBE       _Bit(31)	/* Pend Break Enable */

#define CPUID_FEATURE_SSE3      _HBit(0)	/* Streaming SIMD extensions 3 */
#define CPUID_FEATURE_PCLMULQDQ _HBit(1) /* PCLMULQDQ Instruction */
#define CPUID_FEATURE_DTES64    _HBit(2)  /* 64-bit DS layout */
#define CPUID_FEATURE_MONITOR   _HBit(3)	/* Monitor/mwait */
#define CPUID_FEATURE_DSCPL     _HBit(4)	/* Debug Store CPL */
#define CPUID_FEATURE_VMX       _HBit(5)	/* VMX */
#define CPUID_FEATURE_SMX       _HBit(6)	/* SMX */
#define CPUID_FEATURE_EST       _HBit(7)	/* Enhanced SpeedsTep (GV3) */
#define CPUID_FEATURE_TM2       _HBit(8)	/* Thermal Monitor 2 */
#define CPUID_FEATURE_SSSE3     _HBit(9)	/* Supplemental SSE3 instructions */
#define CPUID_FEATURE_CID       _HBit(10)	/* L1 Context ID */
#define CPUID_FEATURE_SEGLIM64  _HBit(11) /* 64-bit segment limit checking */
#define CPUID_FEATURE_CX16      _HBit(13)	/* CmpXchg16b instruction */
#define CPUID_FEATURE_xTPR      _HBit(14)	/* Send Task PRiority msgs */
#define CPUID_FEATURE_PDCM      _HBit(15)	/* Perf/Debug Capability MSR */

#define CPUID_FEATURE_PCID      _HBit(17) /* ASID-PCID support */
#define CPUID_FEATURE_DCA       _HBit(18)	/* Direct Cache Access */
#define CPUID_FEATURE_SSE4_1    _HBit(19)	/* Streaming SIMD extensions 4.1 */
#define CPUID_FEATURE_SSE4_2    _HBit(20)	/* Streaming SIMD extensions 4.2 */
#define CPUID_FEATURE_xAPIC     _HBit(21)	/* Extended APIC Mode */
#define CPUID_FEATURE_MOVBE     _HBit(22) /* MOVBE instruction */
#define CPUID_FEATURE_POPCNT    _HBit(23)	/* POPCNT instruction */
#define CPUID_FEATURE_TSCTMR    _HBit(24) /* TSC deadline timer */
#define CPUID_FEATURE_AES       _HBit(25)	/* AES instructions */
#define CPUID_FEATURE_XSAVE     _HBit(26) /* XSAVE instructions */
#define CPUID_FEATURE_OSXSAVE   _HBit(27) /* XGETBV/XSETBV instructions */
#define CPUID_FEATURE_AVX1_0	  _HBit(28) /* AVX 1.0 instructions */
#define CPUID_FEATURE_RDRAND	  _HBit(29) /* RDRAND instruction */
#define CPUID_FEATURE_F16C	    _HBit(30) /* Float16 convert instructions */
#define CPUID_FEATURE_VMM       _HBit(31)	/* VMM (Hypervisor) present */

/*
 * Leaf 7, subleaf 0 additional features.
 * Bits returned in %ebx to a CPUID request with {%eax,%ecx} of (0x7,0x0}:
 */
#define CPUID_LEAF7_FEATURE_RDWRFSGS _Bit(0)	/* FS/GS base read/write */
#define CPUID_LEAF7_FEATURE_SMEP     _Bit(7)	/* Supervisor Mode Execute Protect */
#define CPUID_LEAF7_FEATURE_ENFSTRG  _Bit(9)	/* ENhanced Fast STRinG copy */


/*
 * The CPUID_EXTFEATURE_XXX values define 64-bit values
 * returned in %ecx:%edx to a CPUID request with %eax of 0x80000001: 
 */
#define CPUID_EXTFEATURE_SYSCALL   _Bit(11)	/* SYSCALL/sysret */
#define CPUID_EXTFEATURE_XD		     _Bit(20)	/* eXecute Disable */
#define CPUID_EXTFEATURE_1GBPAGE   _Bit(26)     /* 1G-Byte Page support */
#define CPUID_EXTFEATURE_RDTSCP	   _Bit(27)	/* RDTSCP */
#define CPUID_EXTFEATURE_EM64T	   _Bit(29)	/* Extended Mem 64 Technology */

//#define CPUID_EXTFEATURE_LAHF	   _HBit(20)	/* LAFH/SAHF instructions */
// New definition with Snow kernel
#define CPUID_EXTFEATURE_LAHF	   _HBit(0)	/* LAHF/SAHF instructions */
/*
 * The CPUID_EXTFEATURE_XXX values define 64-bit values
 * returned in %ecx:%edx to a CPUID request with %eax of 0x80000007: 
 */
#define CPUID_EXTFEATURE_TSCI      _Bit(8)	/* TSC Invariant */

#define	CPUID_CACHE_SIZE	16	/* Number of descriptor values */

#define CPUID_MWAIT_EXTENSION	_Bit(0)	/* enumeration of WMAIT extensions */
#define CPUID_MWAIT_BREAK	_Bit(1)	/* interrupts are break events	   */

/* Known MSR registers */
#define MSR_IA32_PLATFORM_ID        0x0017	 
#define IA32_APIC_BASE              0x001B  /* used also for AMD */
#define MSR_CORE_THREAD_COUNT       0x0035	 /* limited use - not for Penryn or older	*/
#define IA32_TSC_ADJUST             0x003B   
#define MSR_IA32_BIOS_SIGN_ID       0x008B   /* microcode version */
#define MSR_FSB_FREQ                0x00CD	 /* limited use - not for i7						*/
/*
•	101B: 100 MHz (FSB 400)
•	001B: 133 MHz (FSB 533)
•	011B: 167 MHz (FSB 667)
•	010B: 200 MHz (FSB 800)
•	000B: 267 MHz (FSB 1067)
•	100B: 333 MHz (FSB 1333)
•	110B: 400 MHz (FSB 1600)
 */
// T8300 -> 0x01A2 => 200MHz
#define	MSR_PLATFORM_INFO           0x00CE   /* limited use - MinRatio for i7 but Max for Yonah	*/
                                             /* turbo for penryn */
//haswell
//Low Frequency Mode. LFM is Pn in the P-state table. It can be read at MSR CEh [47:40].
//Minimum Frequency Mode. MFM is the minimum ratio supported by the processor and can be read from MSR CEh [55:48].
#define MSR_PKG_CST_CONFIG_CONTROL  0x00E2   /* sandy and up */
#define MSR_PMG_IO_CAPTURE_BASE     0x00E4  /* sandy and up */
#define IA32_MPERF                  0x00E7   /* TSC in C0 only */
#define IA32_APERF                  0x00E8   /* actual clocks in C0 */
#define MSR_IA32_EXT_CONFIG         0x00EE	 /* limited use - not for i7						*/
#define MSR_FLEX_RATIO              0x0194	 /* limited use - not for Penryn or older			*/
                                             //see no value on most CPUs
#define	MSR_IA32_PERF_STATUS        0x0198
#define MSR_IA32_PERF_CONTROL       0x0199
#define MSR_IA32_CLOCK_MODULATION   0x019A
#define MSR_THERMAL_STATUS          0x019C
#define MSR_IA32_MISC_ENABLE        0x01A0
#define MSR_THERMAL_TARGET          0x01A2	 /* TjMax limited use - not for Penryn or older			*/
#define MSR_TURBO_RATIO_LIMIT       0x01AD	 /* limited use - not for Penryn or older			*/


#define IA32_ENERGY_PERF_BIAS       0x01B0
//MSR 000001B0                                      0000-0000-0000-0005
#define MSR_PACKAGE_THERM_STATUS    0x01B1
//MSR 000001B1                                      0000-0000-8838-0000
#define IA32_PLATFORM_DCA_CAP       0x01F8
//MSR 000001FC                                      0000-0000-0004-005F


// Sandy Bridge & JakeTown specific 'Running Average Power Limit' MSR's.
#define MSR_RAPL_POWER_UNIT			    0x606     /* R/O */
//MSR 00000606                                      0000-0000-000A-1003
#define MSR_PKGC3_IRTL              0x60A    /* RW time limit to go C3 */
          // bit 15 = 1 -- the value valid for C-state PM     
#define MSR_PKGC6_IRTL              0x60B    /* RW time limit to go C6 */
//MSR 0000060B                                      0000-0000-0000-8854
  //Valid + 010=1024ns + 0x54=84mks
#define MSR_PKGC7_IRTL              0x60C    /* RW time limit to go C7 */
//MSR 0000060C                                      0000-0000-0000-8854
#define MSR_PKG_C2_RESIDENCY        0x60D   /* same as TSC but in C2 only */

#define MSR_PKG_RAPL_POWER_LIMIT	  0x610
//MSR 00000610                                      0000-A580-0000-8960
#define MSR_PKG_ENERGY_STATUS		    0x611
//MSR 00000611                                      0000-0000-3212-A857
#define MSR_PKG_POWER_INFO		    	0x614
//MSR 00000614                                      0000-0000-01E0-02F8
// Sandy Bridge IA (Core) domain MSR's.
#define MSR_PP0_POWER_LIMIT			    0x638
#define MSR_PP0_ENERGY_STATUS	    	0x639
#define MSR_PP0_POLICY              0x63A
#define MSR_PP0_PERF_STATUS			    0x63B

// Sandy Bridge Uncore (IGPU) domain MSR's (Not on JakeTown).
#define MSR_PP1_POWER_LIMIT			    0x640
#define MSR_PP1_ENERGY_STATUS	    	0x641
//MSR 00000641                                      0000-0000-0000-0000
#define MSR_PP1_POLICY              0x642

// JakeTown only Memory MSR's.
#define MSR_PKG_PERF_STATUS			    0x613 
#define MSR_DRAM_POWER_LIMIT	    	0x618
#define MSR_DRAM_ENERGY_STATUS	    0x619
#define MSR_DRAM_PERF_STATUS		    0x61B
#define MSR_DRAM_POWER_INFO			    0x61C

//IVY_BRIDGE
#define MSR_CONFIG_TDP_NOMINAL      0x648
#define MSR_CONFIG_TDP_LEVEL1       0x649
#define MSR_CONFIG_TDP_LEVEL2       0x64A
#define MSR_CONFIG_TDP_CONTROL      0x64B  /* write once to lock */
#define MSR_TURBO_ACTIVATION_RATIO  0x64C


//AMD
#define K8_FIDVID_STATUS            0xC0010042
#define K10_COFVID_LIMIT            0xC0010061 /* max enabled p-state (msr >> 4) & 7 */
#define K10_COFVID_CONTROL          0xC0010062 /* switch to p-state */
#define K10_PSTATE_STATUS           0xC0010064
#define K10_COFVID_STATUS           0xC0010071 /* current p-state (msr >> 16) & 7 */
/* specific settings 
static void SavePState(unsigned int index, unsigned int lowMsr, unsigned int core)
{
  CONST unsigned int msrIndex = 0xC0010064u + index;
  CONST DWORD_PTR affinityMask = (DWORD_PTR)1 << core;
  
  DWORD lower, higher;
  RdmsrTx(msrIndex, &lower, &higher, affinityMask);
  
  CONST DWORD lowMsrMask = 0xFE40FFFFu;
  lower = (lower & ~lowMsrMask) | (lowMsr & lowMsrMask);
  
  WrmsrTx(msrIndex, lower, higher, affinityMask);
}

MSR C0010064  8000-0185-0000-1418 [20.00x] [1.4250 V] [13.30 A] [PState Pb0]
MSR C0010065  8000-0185-0000-1615 [18.50x] [1.4125 V] [13.30 A] [PState Pb1]
MSR C0010066  8000-0173-0000-1A1A [21.00x] [1.3875 V] [11.50 A] [PState P0]
MSR C0010067  0000-0173-0000-1A1A
MSR C0010068  0000-0173-0000-181A
MSR C0010069  0000-0173-0000-1A1A
MSR C001006A  8000-0125-0000-604C [ 7.00x] [0.9500 V] [ 3.70 A] [PState P1]
MSR C001006B  0000-0000-0000-0000
*/


#define DEFAULT_FSB                 100000          /* for now, hardcoding 100MHz for old CPUs */


/* CPUID Index */ 
#define CPUID_0		0 
#define CPUID_1		1 
#define CPUID_2		2 
#define CPUID_3		3 
#define CPUID_4		4 
#define CPUID_5		5
#define CPUID_6		6
#define CPUID_80	7
#define CPUID_81	8
#define CPUID_87  9
#define CPUID_88  10
#define CPUID_MAX	16

/* CPU Cache */
#define MAX_CACHE_COUNT  4
#define CPU_CACHE_LEVEL  3

/* PCI */
#define PCI_BASE_ADDRESS_0					0x10		/* 32 bits */
#define PCI_BASE_ADDRESS_1					0x14		/* 32 bits [htype 0,1 only] */
#define PCI_BASE_ADDRESS_2					0x18		/* 32 bits [htype 0 only] */
#define PCI_BASE_ADDRESS_3					0x1c		/* 32 bits */
#define PCI_BASE_ADDRESS_4					0x20		/* 32 bits */
#define PCI_BASE_ADDRESS_5					0x24		/* 32 bits */

#define PCI_CLASS_MEDIA_HDA         0x03

#define GEN_PMCON_1                 0xA0

#define PCIADDR(bus, dev, func)      ((1 << 31) | ((bus) << 16) | ((dev) << 11) | ((func) << 8))
#define REG8(base, reg)              ((volatile UINT8 *)(UINTN)(base))[(reg)]
#define REG16(base, reg)             ((volatile UINT16 *)(UINTN)(base))[(reg) >> 1]
#define REG32(base, reg)             ((volatile UINT32 *)(UINTN)(base))[(reg) >> 2]
#define WRITEREG32(base, reg, value) REG32 ((base), (reg)) = value

#define EFI_HANDLE_TYPE_UNKNOWN                     0x000
#define EFI_HANDLE_TYPE_IMAGE_HANDLE                0x001
#define EFI_HANDLE_TYPE_DRIVER_BINDING_HANDLE       0x002
#define EFI_HANDLE_TYPE_DEVICE_DRIVER               0x004
#define EFI_HANDLE_TYPE_BUS_DRIVER                  0x008
#define EFI_HANDLE_TYPE_DRIVER_CONFIGURATION_HANDLE 0x010
#define EFI_HANDLE_TYPE_DRIVER_DIAGNOSTICS_HANDLE   0x020
#define EFI_HANDLE_TYPE_COMPONENT_NAME_HANDLE       0x040
#define EFI_HANDLE_TYPE_DEVICE_HANDLE               0x080
#define EFI_HANDLE_TYPE_PARENT_HANDLE               0x100
#define EFI_HANDLE_TYPE_CONTROLLER_HANDLE           0x200
#define EFI_HANDLE_TYPE_CHILD_HANDLE                0x400

#define	AML_CHUNK_NONE		      0xff
#define	AML_CHUNK_ZERO		      0x00
#define	AML_CHUNK_ONE           0x01
#define	AML_CHUNK_ALIAS		      0x06
#define	AML_CHUNK_NAME		      0x08
#define	AML_CHUNK_BYTE	      	0x0A
#define	AML_CHUNK_WORD	       	0x0B
#define	AML_CHUNK_DWORD		      0x0C
#define	AML_CHUNK_STRING	      0x0D
#define	AML_CHUNK_QWORD		      0x0E
#define	AML_CHUNK_SCOPE		      0x10
#define	AML_CHUNK_PACKAGE	      0x12
#define	AML_CHUNK_METHOD	      0x14
#define AML_CHUNK_RETURN        0xA4
#define AML_LOCAL0              0x60
#define AML_STORE_OP            0x70
//-----------------------------------
// defines added by pcj
#define	AML_CHUNK_BUFFER	      0x11
#define	AML_CHUNK_STRING_BUFFER	0x15
#define	AML_CHUNK_OP	          0x5B
#define	AML_CHUNK_REFOF	        0x71
#define	AML_CHUNK_DEVICE	      0x82
#define	AML_CHUNK_LOCAL0	      0x60
#define	AML_CHUNK_LOCAL1	      0x61
#define	AML_CHUNK_LOCAL2	      0x62

#define	AML_CHUNK_ARG0	        0x68
#define	AML_CHUNK_ARG1	        0x69
#define	AML_CHUNK_ARG2	        0x6A
#define	AML_CHUNK_ARG3	        0x6B

//DSDT fixes MASK
//0x00FF
#define FIX_DTGP      bit(0)
#define FIX_WARNING   bit(1)
#define FIX_SHUTDOWN  bit(2)
#define FIX_MCHC      bit(3)
#define FIX_HPET      bit(4)
#define FIX_LPC       bit(5)
#define FIX_IPIC      bit(6)
#define FIX_SBUS      bit(7)
//0xFF00
#define FIX_DISPLAY   bit(8)
#define FIX_IDE       bit(9)
#define FIX_SATA      bit(10)
#define FIX_FIREWIRE  bit(11)
#define FIX_USB       bit(12)
#define FIX_LAN       bit(13)
#define FIX_WIFI      bit(14)
#define FIX_HDA       bit(15)
//new bits 16-31 0xFFFF0000
#define FIX_NEW_WAY   bit(31)
//#define FIX_DARWIN    bit(16)
#define FIX_RTC       bit(17)
#define FIX_TMR       bit(18)
#define FIX_IMEI      bit(19)
#define FIX_INTELGFX  bit(20)
#define FIX_WAK       bit(21)
#define FIX_UNUSED    bit(22)
#define FIX_ADP1      bit(23)
#define FIX_PNLF      bit(24)
#define FIX_S3D       bit(25)
#define FIX_ACST      bit(26)
#define FIX_HDMI      bit(27)
#define FIX_REGIONS   bit(28)

//devices
#define DEV_ATI       bit(0)
#define DEV_NVIDIA    bit(1)
#define DEV_INTEL     bit(2)
#define DEV_HDA       bit(3)
#define DEV_HDMI      bit(4)
#define DEV_LAN       bit(5)
#define DEV_WIFI      bit(6)
#define DEV_SATA      bit(7)
#define DEV_IDE       bit(8)
#define DEV_LPC       bit(9)
#define DEV_SMBUS     bit(10)
#define DEV_USB       bit(11)
#define DEV_FIREWIRE  bit(12)
#define DEV_MCHC      bit(13)
#define DEV_IMEI      bit(14)
#define DEV_BY_PCI    bit(31)

#define NUM_OF_CONFIGS 3

// Kernel scan states
#define KERNEL_SCAN_ALL        (0)
#define KERNEL_SCAN_NEWEST     (1)
#define KERNEL_SCAN_OLDEST     (2)
#define KERNEL_SCAN_FIRST      (3)
#define KERNEL_SCAN_LAST       (4)
#define KERNEL_SCAN_MOSTRECENT (5)
#define KERNEL_SCAN_EARLIEST   (6)
#define KERNEL_SCAN_NONE       (100)

// Secure boot policies
// Deny all images
#define SECURE_BOOT_POLICY_DENY      (0)
// Allow all images
#define SECURE_BOOT_POLICY_ALLOW     (1)
// Query the user to choose action
#define SECURE_BOOT_POLICY_QUERY     (2)
// Insert signature into db
#define SECURE_BOOT_POLICY_INSERT    (3)
// White list
#define SECURE_BOOT_POLICY_WHITELIST (4)
// Black list
#define SECURE_BOOT_POLICY_BLACKLIST (5)
// User policy, white and black list with query
#define SECURE_BOOT_POLICY_USER      (6)

struct aml_chunk 
{
  UINT8		          Type;
  UINT8		          pad;
  UINT16		        Length;
  UINT32		        pad2;
  CHAR8		          *Buffer;
  
  UINT16		        Size;
  UINT16		        pad3[3];
  
  struct aml_chunk*	Next;
  struct aml_chunk*	First;
  struct aml_chunk*	Last;
};
typedef struct aml_chunk AML_CHUNK;

struct p_state_vid_fid
{
  UINT8 VID;	// Voltage ID
  UINT8 FID;	// Frequency ID
};

union p_state_control
{
  UINT16 Control;
  struct p_state_vid_fid VID_FID;
};

struct p_state 
{
  union p_state_control Control;
  
  UINT32 CID;		// Compare ID
  UINT32 Frequency;
};
typedef struct p_state P_STATE;

struct _oper_region {
  CHAR8 Name[8];
  UINT32 Address;
  struct _oper_region *next;
};
typedef struct _oper_region OPER_REGION;


typedef enum {
  kTagTypeNone,
  kTagTypeDict,
  kTagTypeKey,
  kTagTypeString,
  kTagTypeInteger,
  kTagTypeData,
  kTagTypeDate,
  kTagTypeFalse,
  kTagTypeTrue,
  kTagTypeArray
} TAG_TYPE;

typedef struct _DRIVERS_FLAGS {
  BOOLEAN EmuVariableLoaded;
  BOOLEAN VideoLoaded;
  BOOLEAN PartitionLoaded;
  BOOLEAN MemFixLoaded;
  BOOLEAN AptioFixLoaded;
  BOOLEAN AptioFix2Loaded;
  BOOLEAN HFSLoaded;
} DRIVERS_FLAGS;

#pragma pack(push)
#pragma pack(1)

struct Symbol {
  UINTN         refCount;
  struct Symbol *next;
  CHAR8         string[1];
};

typedef struct Symbol Symbol, *SymbolPtr;

typedef struct {
  
  UINTN	type;
  CHAR8 *string;
  UINT8	*data;
  UINTN	dataLen;
  UINTN offset;
  VOID	*tag;
  VOID	*tagNext;
  
} TagStruct, *TagPtr;

typedef struct {
  
  EFI_ACPI_DESCRIPTION_HEADER Header;
  UINT32						          Entry;
  
} RSDT_TABLE;

typedef struct {
  
  EFI_ACPI_DESCRIPTION_HEADER Header;
  UINT64						          Entry;
  
} XSDT_TABLE;
/*
typedef struct {
  UINT32  Data1;
  UINT16  Data2;
  UINT16  Data3;
  UINT8   Data4[8];
} GUID;
*/

typedef struct DEV_PROPERTY DEV_PROPERTY;
struct DEV_PROPERTY {
  UINT32 Device;
  CHAR8  *Key;
  CHAR8  *Value;
  UINTN  ValueLen;
  DEV_PROPERTY *Next;
};

typedef struct CUSTOM_LOADER_ENTRY CUSTOM_LOADER_ENTRY;
struct CUSTOM_LOADER_ENTRY {
  CUSTOM_LOADER_ENTRY     *Next;
  CUSTOM_LOADER_ENTRY     *SubEntries;
  EG_IMAGE                *Image;
  EG_IMAGE                *DriveImage;
  CHAR16                  *ImagePath;
  CHAR16                  *DriveImagePath;
  CHAR16                  *Volume;
  CHAR16                  *Path;
  CHAR16                  *Options;
  CHAR16                  *FullTitle;
  CHAR16                  *Title;
  CHAR16                  *Settings;
  CHAR16                  Hotkey;
  BOOLEAN                 CommonSettings; 
  UINT8                   Flags;
  UINT8                   Type;
  UINT8                   VolumeType;
  UINT8                   KernelScan;
  UINT8                   CustomBoot;
  EG_IMAGE                *CustomLogo;
  EG_PIXEL                *BootBgColor;
  KERNEL_AND_KEXT_PATCHES KernelAndKextPatches;
};

typedef struct CUSTOM_LEGACY_ENTRY CUSTOM_LEGACY_ENTRY;
struct CUSTOM_LEGACY_ENTRY {
  CUSTOM_LEGACY_ENTRY *Next;
  EG_IMAGE            *Image;
  EG_IMAGE            *DriveImage;
  CHAR16              *ImagePath;
  CHAR16              *DriveImagePath;
  CHAR16              *Volume;
  CHAR16              *FullTitle;
  CHAR16              *Title;
  CHAR16              Hotkey;
  UINT8               Flags;
  UINT8               Type;
  UINT8               VolumeType;
};

typedef struct CUSTOM_TOOL_ENTRY CUSTOM_TOOL_ENTRY;
struct CUSTOM_TOOL_ENTRY {
  CUSTOM_TOOL_ENTRY *Next;
  EG_IMAGE          *Image;
  CHAR16            *ImagePath;
  CHAR16            *Volume;
  CHAR16            *Path;
  CHAR16            *Options;
  CHAR16            *FullTitle;
  CHAR16            *Title;
  CHAR16            Hotkey;
  UINT8             Flags;
  UINT8             VolumeType;
};

typedef struct ACPI_DROP_TABLE ACPI_DROP_TABLE;
struct ACPI_DROP_TABLE
{
  ACPI_DROP_TABLE *Next;
  UINT32          Signature;
  UINT32          Length;
  UINT64          TableId;
  INPUT_ITEM      MenuItem;
};

typedef struct {
  
  // SMBIOS TYPE0
  CHAR8                 	VendorName[64];
  CHAR8	                  RomVersion[64];
  CHAR8	                  ReleaseDate[64];
  // SMBIOS TYPE1
  CHAR8                 	ManufactureName[64];
  CHAR8                 	ProductName[64];
  CHAR8                 	VersionNr[64];
  CHAR8	                  SerialNr[64];
  EFI_GUID                SmUUID;
  BOOLEAN                 SmUUIDConfig;
  CHAR8                   pad0[7];
//CHAR8	                  Uuid[64];
//CHAR8	                  SKUNumber[64];
  CHAR8                 	FamilyName[64];
  CHAR8                   OEMProduct[64];
  CHAR8                   OEMVendor[64];
  // SMBIOS TYPE2
  CHAR8	                  BoardManufactureName[64];
  CHAR8	                  BoardSerialNumber[64];
  CHAR8                 	BoardNumber[64]; //Board-ID
  CHAR8                 	LocationInChassis[64];
  CHAR8                   BoardVersion[64];
  CHAR8                   OEMBoard[64];
  UINT8                   BoardType;
  UINT8                   Pad1;
  // SMBIOS TYPE3
  BOOLEAN                 Mobile;
  UINT8                   ChassisType;
  CHAR8	                  ChassisManufacturer[64];
  CHAR8	                  ChassisAssetTag[64];
  // SMBIOS TYPE4
  UINT32	                CpuFreqMHz;
  UINT32	                BusSpeed; //in kHz
  BOOLEAN                 Turbo;
  UINT8                   EnabledCores;
  BOOLEAN                 UserChange;
  BOOLEAN                 QEMU;
  // SMBIOS TYPE17
  CHAR8                 	MemoryManufacturer[64];
  CHAR8                 	MemorySerialNumber[64];
  CHAR8                 	MemoryPartNumber[64];
  CHAR8                 	MemorySpeed[64];
  // SMBIOS TYPE131
  UINT16                  CpuType;
  // SMBIOS TYPE132
  UINT16                  QPI;
  BOOLEAN                 TrustSMBIOS;
  BOOLEAN                 InjectMemoryTables;
  INT8                    XMPDetection;
  INT8                    reserved;
  // OS parameters
  CHAR8                   Language[16];
  CHAR8                   BootArgs[256];
  CHAR16                  CustomUuid[40];
  CHAR16                  *DefaultVolume;
#if defined(MDE_CPU_IA32)
  UINT32                  align10;
#endif
  BOOLEAN                 LastBootedVolume;
  UINT8                   Pad21[7];
  
  CHAR16                  *DefaultLoader;
#if defined(MDE_CPU_IA32)
  UINT32                  align11;
#endif
  UINT16                  BacklightLevel;
  BOOLEAN                 BacklightLevelConfig;
  BOOLEAN                 IntelBacklight;
  BOOLEAN                 MemoryFix;
  BOOLEAN                 WithKexts;
  BOOLEAN                 WithKextsIfNoFakeSMC;
  BOOLEAN                 FakeSMCFound;
  BOOLEAN                 NoCaches;
  
  // GUI parameters
  BOOLEAN                 Debug;
  BOOLEAN                 Proportional;
  UINT8                   Pad22[5];
  
  //ACPI
  UINT64	              ResetAddr;
  UINT8 	              ResetVal;
  BOOLEAN	              UseDSDTmini;
  BOOLEAN                 DropSSDT;
  BOOLEAN	              GeneratePStates;
  BOOLEAN                 GenerateCStates;
  UINT8                   PLimitDict;
  UINT8                   UnderVoltStep;
  BOOLEAN                 DoubleFirstState;
  BOOLEAN                 SuspendOverride;
  BOOLEAN                 EnableC2;
  BOOLEAN                 EnableC4;
  BOOLEAN                 EnableC6;
  BOOLEAN                 EnableISS;
  BOOLEAN                 SlpSmiEnable;
  UINT16                  C3Latency;
  BOOLEAN                 smartUPS;
  BOOLEAN                 PatchNMI;
  BOOLEAN                 EnableC7;
  UINT8                   SavingMode;
  
  CHAR16                  DsdtName[60];
  UINT32                  FixDsdt;
  UINT8                   MinMultiplier;
  UINT8                   MaxMultiplier;
  UINT8                   PluginType;
  BOOLEAN                 DropMCFG;
  
  //Injections
  BOOLEAN                 StringInjector;
  BOOLEAN                 InjectSystemID;
  BOOLEAN                 NoDefaultProperties;
  
  BOOLEAN                 ReuseFFFF;
    
  //PCI devices
  UINT32                  FakeATI;    //97
  UINT32                  FakeNVidia;
  UINT32                  FakeIntel;
  UINT32                  FakeLAN;   //100
  UINT32                  FakeWIFI;
  UINT32                  FakeSATA;
  UINT32                  FakeXHCI;  //103
  UINT32                  FakeIMEI;  //106
  
  //Graphics
  UINT16                  PCIRootUID;
  BOOLEAN                 GraphicsInjector;
  BOOLEAN                 InjectIntel;
  BOOLEAN                 InjectATI;
  BOOLEAN                 InjectNVidia;
  BOOLEAN                 LoadVBios;
  BOOLEAN                 PatchVBios;
  VBIOS_PATCH_BYTES       *PatchVBiosBytes;
#if defined(MDE_CPU_IA32)
  UINT32                  align12;
#endif
  
  UINTN                   PatchVBiosBytesCount;
#if defined(MDE_CPU_IA32)
  UINT32                  align1;
#endif
  BOOLEAN                 InjectEDID;
  BOOLEAN                 LpcTune;
  UINT16                  DropOEM_DSM;
  UINT8                   *CustomEDID;
#if defined(MDE_CPU_IA32)
  UINT32  align13;
#endif
  
  CHAR16                  FBName[16];
  UINT16                  VideoPorts;
  BOOLEAN                 NvidiaGeneric;
  BOOLEAN                 NvidiaSingle;
  UINT64                  VRAM;
  UINT8                   Dcfg[8];
  UINT8                   NVCAP[20];
  UINT8                   pad41[4];
  UINT32                  DualLink;
  UINT32                  IgPlatform;
  
  // Secure boot white/black list
  UINT32                  SecureBootWhiteListCount;
  UINT32                  SecureBootBlackListCount;
  CHAR16                  **SecureBootWhiteList;
#if defined(MDE_CPU_IA32)
  UINT32                  align14;
#endif
  
  CHAR16                  **SecureBootBlackList;
#if defined(MDE_CPU_IA32)
  UINT32                  align15;
#endif
  
  // Secure boot
  UINT8                   SecureBoot;
  UINT8                   SecureBootSetupMode;
  UINT8                   SecureBootPolicy;

  // HDA
  BOOLEAN                 HDAInjection;
  INT32                   HDALayoutId;
  
  // USB DeviceTree injection
  BOOLEAN USBInjection;
  // USB ownership fix
  BOOLEAN                 USBFixOwnership;
  BOOLEAN                 InjectClockID;
  BOOLEAN                 HighCurrent;
  UINT8                   pad61[4];
  
  // LegacyBoot
  CHAR16                  LegacyBoot[32];
  UINT16                  LegacyBiosDefaultEntry;
  UINT8                   pad62[6];
  
  //Volumes hiding
  CHAR16                  **HVHideStrings;
#if defined(MDE_CPU_IA32)
  UINT32  align191;
#endif
  
  INTN                    HVCount;
#if defined(MDE_CPU_IA32)
  UINT32  align4;
#endif
  
  // KernelAndKextPatches
  KERNEL_AND_KEXT_PATCHES KernelAndKextPatches;
  BOOLEAN                 KextPatchesAllowed;

  //Pointer
  BOOLEAN                 PointerEnabled;
  UINT8                   pad82[6];
  INTN                    PointerSpeed;
#if defined(MDE_CPU_IA32)
  UINT32                  align5;
#endif
  UINT64                  DoubleClickTime;
  BOOLEAN                 PointerMirror;
  UINT8                   pad7[2];
  UINT8                   CustomBoot;
  EG_IMAGE                *CustomLogo;

  //SpoofOSXVersion
  CHAR16                  *SpoofOSXVersion;
  
  UINT32                  RefCLK;
  
  // RtVariables
  CHAR8                   *RtMLB;
#if defined(MDE_CPU_IA32)
  UINT32                  align20;
#endif
  
  UINT8                   *RtROM;
#if defined(MDE_CPU_IA32)
  UINT32                  align21;
#endif
  UINTN                   RtROMLen;
#if defined(MDE_CPU_IA32)
  UINT32                  align6;
#endif

  UINT32                  CsrActiveConfig;
  UINT16                  BooterConfig;
  UINT8                   pad71[2];
  
  // Multi-config
  CHAR16  *ConfigName;
#if defined(MDE_CPU_IA32)
  UINT32                  align24;
#endif
  
  CHAR16  *MainConfigName;
#if defined(MDE_CPU_IA32)
  UINT32                  align25;
#endif
  
  //Drivers
  INTN    BlackListCount;
#if defined(MDE_CPU_IA32)
  UINT32                  align7;
#endif
  CHAR16                  **BlackList;
#if defined(MDE_CPU_IA32)
  UINT32                  align26;
#endif
  
  //SMC keys
  CHAR8                   RPlt[8];
  CHAR8                   RBr[8];
  UINT8                   EPCI[4];
  UINT8                   REV[6];
  
  //other devices
  BOOLEAN                 Rtc8Allowed;
  BOOLEAN                 ForceHPET;
  BOOLEAN                 ResetHDA;
  UINT8                   pad8[3];
  UINT32                  DisableFunctions;

  //Patch DSDT arbitrary
  UINT32                  PatchDsdtNum;
  UINT8                   **PatchDsdtFind;
#if defined(MDE_CPU_IA32)
  UINT32                  align27;
#endif
  
  UINT32 *LenToFind;
#if defined(MDE_CPU_IA32)
  UINT32                  align28;
#endif
  
  UINT8  **PatchDsdtReplace;
#if defined(MDE_CPU_IA32)
  UINT32                  align29;
#endif
  
  UINT32 *LenToReplace;
#if defined(MDE_CPU_IA32)
  UINT32                  align30;
#endif
  
  BOOLEAN                 DebugDSDT;
  BOOLEAN                 SlpWak;
  BOOLEAN                 UseIntelHDMI;
  UINT8                   AFGLowPowerState;
  UINT8                   pad83[4];

  // Table dropping
  ACPI_DROP_TABLE         *ACPIDropTables;
#if defined(MDE_CPU_IA32)
  UINT32  align32;
#endif
  

  // Custom entries
  BOOLEAN                 DisableEntryScan;
  BOOLEAN                 DisableToolScan;
  BOOLEAN                 ShowHiddenEntries;
  UINT8                   KernelScan;
  BOOLEAN                 LinuxScan;
  UINT8                   pad84[3];
  CUSTOM_LOADER_ENTRY     *CustomEntries;
#if defined(MDE_CPU_IA32)
  UINT32                  align33;
#endif
  CUSTOM_LEGACY_ENTRY     *CustomLegacy;
#if defined(MDE_CPU_IA32)
  UINT32                  align34;
#endif
  CUSTOM_TOOL_ENTRY       *CustomTool;
#if defined(MDE_CPU_IA32)
  UINT32                  align35;
#endif
  
  //Add custom properties
  INTN                    NrAddProperties;
#if defined(MDE_CPU_IA32)
  UINT32  align8;
#endif
  DEV_PROPERTY            *AddProperties;
#if defined(MDE_CPU_IA32)
  UINT32                  align31;
#endif

  //BlackListed kexts
  CHAR16                  BlockKexts[64];
  
  //ACPI tables
  UINTN    SortedACPICount;
#if defined(MDE_CPU_IA32)
  UINT32                  align37;
#endif
  CHAR16                  **SortedACPI;
#if defined(MDE_CPU_IA32)
  UINT32                  align38;
#endif

  
} SETTINGS_DATA;

typedef struct {
 //values from CPUID 
  UINT32                  CPUID[CPUID_MAX][4];
  UINT32                  Vendor;
  UINT32                  Signature;
  UINT32                  Family;
  UINT32                  Model;
  UINT32                  Stepping;
  UINT32                  Type;
  UINT32                  Extmodel;
  UINT32                  Extfamily;
  UINT64                  Features;
  UINT64                  ExtFeatures;
  UINT32                  CoresPerPackage;
  UINT32                  LogicalPerPackage;
  CHAR8                   BrandString[48];
  
  //values from BIOS
  UINT32                  ExternalClock; //keep this values as kHz
  UINT32                  MaxSpeed;       //MHz
  UINT32                  CurrentSpeed;   //MHz
  UINT32                  Pad;
  
  //calculated from MSR
  UINT64                  MicroCode;
  UINT64                  ProcessorFlag;
  UINT32                  MaxRatio;
  UINT32                  SubDivider;
  UINT32                  MinRatio;
  UINT32                  DynFSB;
  UINT64                  ProcessorInterconnectSpeed; //MHz
  UINT64                  FSBFrequency; //Hz
  UINT64                  CPUFrequency;
  UINT64                  TSCFrequency;
  UINT8                   Cores;
  UINT8                   EnabledCores;
  UINT8                   Threads;
  UINT8                   Mobile;  //not for i3-i7
  BOOLEAN                 Turbo;
  UINT8                   Pad2[3];

  /* Core i7,5,3 */
  UINT16                  Turbo1; //1 Core
  UINT16                  Turbo2; //2 Core
  UINT16                  Turbo3; //3 Core
  UINT16                  Turbo4; //4 Core
  
  UINT64                  TSCCalibr;
    
} CPU_STRUCTURE;

typedef enum {
  
  MacBook11,
  MacBook21,
  MacBook41,
  MacBook52,
  MacBookPro51,
  MacBookPro62,
  MacBookPro81,
  MacBookPro83,
  MacBookPro92,
  MacBookPro101,
  MacBookPro111,
  MacBookAir31,
  MacBookAir52,
  MacBookAir62,
  MacMini21,
  MacMini51,
  MacMini62,
  iMac81,
  iMac101,
  iMac111,
  iMac112,
  iMac113,
  iMac121,
  iMac122,
  iMac131,
  iMac132,
  iMac141,
  iMac142,
  iMac151,
  iMac171,
  MacPro31,
  MacPro41,
  MacPro51,
  MacPro61,
  
  MaxMachineType
  
} MACHINE_TYPES;

typedef struct {
  BOOLEAN	InUse;
  UINT8   Type;
  UINT16  pad0;
  UINT32  pad1;
  UINT32	ModuleSize;
  UINT32	Frequency;
  CHAR8*	Vendor;
  CHAR8*	PartNo;
  CHAR8*	SerialNo;
} RAM_SLOT_INFO; 

// The maximum number of RAM slots to detect
// even for 3-channels chipset X58 there are no more then 8 slots
#define MAX_RAM_SLOTS 24
// The maximum sane frequency for a RAM module
#define MAX_RAM_FREQUENCY 5000

typedef struct {
  
  UINT64        Frequency;
  UINT32        Divider;
  UINT8         TRC;
  UINT8         TRP;
  UINT8         RAS;
  UINT8         Channels;
  UINT8         Slots;
  UINT8         Type;
  UINT8         SPDInUse;
  UINT8         SMBIOSInUse;
  UINT8         UserInUse;
  UINT8         UserChannels;
  UINT8         pad[2];

  RAM_SLOT_INFO SPD[MAX_RAM_SLOTS * 4];
  RAM_SLOT_INFO SMBIOS[MAX_RAM_SLOTS * 4];
  RAM_SLOT_INFO User[MAX_RAM_SLOTS * 4];

} MEM_STRUCTURE;
//unused
/*
typedef struct {
  UINT8     MaxMemorySlots;			// number of memory slots polulated by SMBIOS
  UINT8     CntMemorySlots;			// number of memory slots counted
  UINT16		MemoryModules;			// number of memory modules installed
  UINT8		DIMM[MAX_RAM_SLOTS];	// Information and SPD mapping for each slot
} DMI;
*/

typedef enum {
  english = 0,  //en
  russian,    //ru
  french,     //fr
  german,     //de
  dutch,      //nl
  italian,    //it
  spanish,    //es
  portuguese, //pt
  brasil,     //br
  polish,     //pl
  ukrainian,  //ua
  croatian,   //hr
  czech,      //cs
  indonesian, //id
  korean,     //ko
  chinese,    //cn
  romanian    //ro
  //something else? add, please
} LANGUAGES;

typedef enum {
  Unknown,
  Ati,
  Intel,
  Nvidia
  
} GFX_MANUFACTERER;

typedef struct {
  GFX_MANUFACTERER  Vendor;
  UINT8             Ports;  
  UINT16            DeviceID;
  UINT16            Family;
//UINT16            Width;
//UINT16            Height;
  CHAR8             Model[64];
  CHAR8             Config[64];
  BOOLEAN           LoadVBios;
//BOOLEAN           PatchVBios;
  UINTN             Segment;
  UINTN             Bus;
  UINTN             Device;
  UINTN             Function;
  EFI_HANDLE        Handle;
} GFX_PROPERTIES;

typedef struct {
  UINT16            SegmentGroupNum;
  UINT8             BusNum;
  UINT8             DevFuncNum;  
  BOOLEAN           Valid;
//UINT8             DeviceN;
  UINT8             SlotID;
  UINT8             SlotType;
  CHAR8             SlotName[31];
} SLOT_DEVICE;

typedef struct {	
  UINT32            Signature;
  LIST_ENTRY        Link;
  CHAR8             Model[64];
  UINT32            Id;
  UINT32            SubId;
  UINT64            VideoRam;
} CARDLIST;

typedef struct {
    ///
    /// XXXX in BootXXXX.
    ///
  UINT16                     BootNum;
    ///
    /// Pointer to raw EFI_LOAD_OPTION (BootXXXX) variable content.
    ///
    VOID                     *Variable;
    ///
    /// Variable size in bytes.
    ///
    UINTN                    VariableSize;
    ///
    /// BootOption Attributes (first 4 bytes from Variable).
    ///
    UINT32                   Attributes;
    ///
    /// BootOption FilePathListLength (next 2 bytes from Variable).
    ///
    UINT16                   FilePathListLength;
    ///
    /// Null terminated BootOption Description (pointer to 6th byte of Variable).
    ///
    CHAR16                   *Description;
    ///
    /// Size in bytes of BootOption Description.
    ///
    UINTN                    DescriptionSize;
    ///
    /// Pointer to BootOption FilePathList.
    ///
    EFI_DEVICE_PATH_PROTOCOL *FilePathList;
    ///
    /// Pointer to BootOption OptionalData.
    ///
    UINT8                    *OptionalData;
    ///
    /// BootOption OptionalData size in bytes.
    ///
    UINTN                    OptionalDataSize;
} BO_BOOT_OPTION;

#define CARDLIST_SIGNATURE SIGNATURE_32('C','A','R','D')


#pragma pack(pop)
//extern CHAR8                          *msgbuf;
//extern CHAR8                          *msgCursor;
extern SMBIOS_STRUCTURE_POINTER	      SmbiosTable;
extern GFX_PROPERTIES                 gGraphics[];
extern UINTN                          NGFX;
extern BOOLEAN                        gMobile;
extern BOOLEAN                        DoHibernateWake;
//extern UINT32                         gCpuSpeed;  //kHz
//extern UINT16                         gCPUtype;
extern UINT64                         TurboMsr;
extern CHAR8                          *BiosVendor;
extern EFI_GUID                       *gEfiBootDeviceGuid;
extern EFI_DEVICE_PATH_PROTOCOL       *gEfiBootDeviceData;
extern CHAR8                          *AppleSystemVersion[];
extern CHAR8                          *AppleFirmwareVersion[];
extern CHAR8                          *AppleReleaseDate[];
extern CHAR8                          *AppleManufacturer;
extern CHAR8                          *AppleProductName[];
extern CHAR8                          *AppleSystemVersion[];
extern CHAR8                          *AppleSerialNumber[];
extern CHAR8                          *AppleFamilies[];
extern CHAR8                          *AppleBoardID[];
extern CHAR8                          *AppleChassisAsset[];
extern CHAR8                          *AppleBoardSN;
extern CHAR8                          *AppleBoardLocation;
extern EFI_SYSTEM_TABLE               *gST;
extern EFI_BOOT_SERVICES              *gBS;
extern SETTINGS_DATA                  gSettings;
extern LANGUAGES                      gLanguage;
extern BOOLEAN                        gFirmwareClover;
extern DRIVERS_FLAGS                  gDriversFlags;
extern UINT32                         gFwFeatures;
extern CPU_STRUCTURE                  gCPUStructure;
extern EFI_GUID                       gUuid;
extern SLOT_DEVICE                    SlotDevices[];
extern EFI_EDID_DISCOVERED_PROTOCOL   *EdidDiscovered;
extern UINT8                          *gEDID;
extern UINT32                         mPropSize;
extern UINT8                          *mProperties;
extern CHAR8                          *gDeviceProperties;
extern UINT32                         cPropSize;
extern UINT8                          *cProperties;
extern CHAR8                          *cDeviceProperties;
extern INPUT_ITEM                     *InputItems;
//extern EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput;

extern EFI_GUID	                      gEfiAppleBootGuid;
extern EFI_GUID	                      gEfiAppleNvramGuid;
extern EFI_GUID	                      AppleSystemInfoProducerName;
extern EFI_GUID	                      AppleDevicePropertyProtocolGuid;
extern EFI_GUID	                      gAppleScreenInfoProtocolGuid;
extern EFI_GUID	                      gEfiAppleVendorGuid;
extern EFI_GUID	                      gEfiPartTypeSystemPartGuid;
extern EFI_GUID	                      gMsgLogProtocolGuid;
extern EFI_GUID	                      gEfiLegacy8259ProtocolGuid;

extern EFI_EVENT	                    mVirtualAddressChangeEvent;
extern EFI_EVENT	                    OnReadyToBootEvent;
extern EFI_EVENT	                    ExitBootServiceEvent;
extern EFI_EVENT	                    mSimpleFileSystemChangeEvent;
extern UINTN	                        gEvent;

extern UINT16	                        gBacklightLevel;
//mouse
extern ACTION	                        gAction;
extern UINTN	                        gItemID;

//CHAR8*   orgBiosDsdt;
extern UINT64	                        BiosDsdt;
extern UINT32	                        BiosDsdtLen;
extern UINT8	                        acpi_cpu_count;
extern CHAR8	                        *acpi_cpu_name[32];
extern CHAR8	                        *acpi_cpu_score;
extern BOOLEAN	                      SSSE3;
extern BOOLEAN	                      defDSM;
extern UINT16	                        dropDSM;

extern TagPtr                         gConfigDict[];
//-----------------------------------

VOID
FixBiosDsdt (
  UINT8                                     *Dsdt,
  EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE *fadt,
  CHAR8                                     *OSVersion
  );

VOID
GetBiosRegions (
  EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE *fadt
  );

INT32
FindBin (
  UINT8  *Array,
  UINT32 ArrayLen,
  UINT8  *Pattern,
  UINT32 PatternLen
  );

EFI_STATUS
MouseBirth ();

VOID
KillMouse ();

VOID
HidePointer ();

/*
EFI_STATUS
WaitForInputEvent (
  REFIT_MENU_SCREEN *Screen,
  UINTN             TimeoutDefault
  );
*/

EFI_STATUS
WaitForInputEventPoll (
  REFIT_MENU_SCREEN *Screen,
  UINTN             TimeoutDefault
  );

//VOID
//WaitForSts ();

//EFI_STATUS
//ApplySettings ();

VOID
InitBooterLog ();

EFI_STATUS
SetupBooterLog (
  BOOLEAN AllowGrownSize
  );

EFI_STATUS
SaveBooterLog (
  IN  EFI_FILE_HANDLE BaseDir  OPTIONAL,
  IN  CHAR16 *FileName
  );

VOID
DebugLog (
  IN        INTN  DebugMode,
  IN  CONST CHAR8 *FormatString, ...);

/** Prints series of bytes. */
VOID
PrintBytes (
  IN  VOID *Bytes,
  IN  UINTN Number
  );

VOID
SetDMISettingsForModel (
  MACHINE_TYPES Model
  );

MACHINE_TYPES GetModelFromString (
  CHAR8 *ProductName
  );

VOID
GetDefaultSettings();

VOID
FillInputs (
  BOOLEAN New
  );

VOID
ApplyInputs ();


BOOLEAN
IsValidGuidAsciiString (
  IN CHAR8 *Str
  );

EFI_STATUS
StrToGuid (
  IN      CHAR16   *Str,
     OUT  EFI_GUID *Guid);

EFI_STATUS
StrToGuidLE (
  IN      CHAR16   *Str,
     OUT  EFI_GUID *Guid);


EFI_STATUS
InitBootScreen (
  IN  LOADER_ENTRY *Entry
  );

EFI_STATUS
InitializeConsoleSim ();

EFI_STATUS
GuiEventsInitialize ();

EFI_STATUS
InitializeEdidOverride ();

UINT8*
getCurrentEdid ();

EFI_STATUS
GetEdidDiscovered ();

//Settings.c
UINT32
GetCrc32 (
  UINT8 *Buffer,
  UINTN Size
  );

VOID
GetCPUProperties ();

VOID
GetDevices();

MACHINE_TYPES
GetDefaultModel ();

UINT16
GetAdvancedCpuType ();

CHAR8
*GetOSVersion (
  IN  LOADER_ENTRY *Entry
  );

CHAR16
*GetOSIconName (
  IN  CHAR8 *OSVersion
  );

EFI_STATUS
GetRootUUID (
  IN  REFIT_VOLUME *Volume
  );

EFI_STATUS
GetEarlyUserSettings (
  IN  EFI_FILE *RootDir,
      TagPtr   CfgDict
  );

EFI_STATUS
GetUserSettings (
  IN  EFI_FILE *RootDir,
      TagPtr CfgDict
  );

EFI_STATUS
InitTheme (
  BOOLEAN  UseThemeDefinedInNVRam,
  EFI_TIME *Time
  );

EFI_STATUS
SetFSInjection (
  IN LOADER_ENTRY *Entry
  );

CHAR16*
GetOtherKextsDir ();

CHAR16*
GetOSVersionKextsDir (
  CHAR8 *OSVersion
  );

EFI_STATUS
InjectKextsFromDir (
  EFI_STATUS Status,
  CHAR16 *SrcDir
  );

EFI_STATUS
LoadKexts (
  IN  LOADER_ENTRY *Entry
  );

VOID
ParseLoadOptions (
  OUT  CHAR16 **Conf,
  OUT  TagPtr *Dict
  );

//
// Nvram.c
//
VOID
*GetNvramVariable (
  IN      CHAR16   *VariableName,
  IN      EFI_GUID *VendorGuid,
     OUT  UINT32   *Attributes    OPTIONAL,
     OUT  UINTN    *DataSize      OPTIONAL
     );

EFI_STATUS
AddNvramVariable (
  IN  CHAR16   *VariableName,
  IN  EFI_GUID *VendorGuid,
  IN  UINT32   Attributes,
  IN  UINTN    DataSize,
  IN  VOID     *Data
  );

EFI_STATUS
SetNvramVariable (
  IN  CHAR16   *VariableName,
  IN  EFI_GUID *VendorGuid,
  IN  UINT32   Attributes,
  IN  UINTN    DataSize,
  IN  VOID     *Data
  );

EFI_STATUS
DeleteNvramVariable (
  IN  CHAR16   *VariableName,
  IN  EFI_GUID *VendorGuid
  );

EFI_STATUS
GetEfiBootDeviceFromNvram ();

EFI_GUID
*FindGPTPartitionGuidInDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath
  );

VOID
PutNvramPlistToRtVars ();

VOID
GetSmcKeys ();

VOID
GetMacAddress();

INTN
FindStartupDiskVolume (
  REFIT_MENU_SCREEN *MainMenu
  );

EFI_STATUS
SetStartupDiskVolume (
  IN  REFIT_VOLUME *Volume,
  IN  CHAR16       *LoaderPath
  );

VOID
RemoveStartupDiskVolume ();

UINT64
GetEfiTimeInMs (IN EFI_TIME *T);


EFI_STATUS
LogDataHub (
  EFI_GUID *TypeGuid,
  CHAR16   *Name,
  VOID     *Data,
  UINT32   DataSize
  );

EFI_STATUS
SetVariablesForOSX ();

VOID
SetupDataForOSX ();

EFI_STATUS
SetPrivateVarProto ();

VOID
SetDevices (
  LOADER_ENTRY *Entry
  );

VOID
ScanSPD ();

BOOLEAN
setup_ati_devprop (
  LOADER_ENTRY *Entry,
  pci_dt_t     *ati_dev
  );

BOOLEAN
setup_gma_devprop (
  pci_dt_t *gma_dev
  );

CHAR8
*get_gma_model (
  IN UINT16 DeviceID
  );

BOOLEAN
setup_nvidia_devprop (
  pci_dt_t *nvda_dev
  );

CHAR8
*get_nvidia_model (
  UINT32 device_id,
  UINT32 subsys_id
  );

VOID
FillCardList();

CARDLIST
*FindCardWithIds (
  UINT32 Id,
  UINT32 SubId
  );

VOID
AddCard (
  CONST CHAR8 *Model,
  UINT32      Id,
  UINT32      SubId,
  UINT64      VideoRam);

EG_IMAGE
*egDecodePNG (
  IN UINT8 *FileData,
  IN UINTN FileDataLength,
  IN UINTN IconSize,
  IN BOOLEAN WantAlpha
  );

//ACPI
EFI_STATUS
PatchACPI(IN REFIT_VOLUME *Volume, CHAR8 *OSVersion);

EFI_STATUS
PatchACPI_OtherOS(CHAR16* OsSubdir, BOOLEAN DropSSDT);

UINT8
Checksum8 (
  VOID *startPtr,
  UINT32 len
  );

BOOLEAN
tableSign (
  CHAR8       *table,
  CONST CHAR8 *sgn);

VOID
SaveOemDsdt (
  BOOLEAN FullPatch
  );

VOID
SaveOemTables ();

EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE
*GetFadt ();

UINT32
FixAny (
  UINT8* dsdt,
  UINT32 len,
  UINT8* ToFind,
  UINT32 LenTF,
  UINT8* ToReplace,
  UINT32 LenTR
  );

VOID
GetAcpiTablesList ();

EFI_STATUS
EventsInitialize (
  IN LOADER_ENTRY *Entry
  );

EFI_STATUS
EjectVolume (
  IN REFIT_VOLUME *Volume
  );

EFI_STATUS
bootElTorito (
  IN REFIT_VOLUME *volume
  );

EFI_STATUS
bootMBR (
  IN REFIT_VOLUME *volume
  );

EFI_STATUS
bootPBR (
  IN REFIT_VOLUME *volume
  );

EFI_STATUS
bootPBRtest (
  IN REFIT_VOLUME *volume
  );

EFI_STATUS
bootLegacyBiosDefault (
  IN  UINT16 LegacyBiosDefaultEntry
  );

VOID
DumpBiosMemoryMap ();

CHAR8*
XMLDecode (
  CHAR8 *src
  );

EFI_STATUS
ParseXML (
  CONST CHAR8  *buffer,
        TagPtr *dict,
        UINT32 bufSize
  );

TagPtr
GetProperty (
        TagPtr dict,
  CONST CHAR8* key
  );

EFI_STATUS
XMLParseNextTag (
  CHAR8  *buffer,
  TagPtr *tag,
  UINT32 *lenPtr
  );

VOID
FreeTag (
  TagPtr tag
  );

EFI_STATUS
GetNextTag (
  UINT8  *buffer,
  CHAR8  **tag,
  UINT32 *start,
  UINT32 *length
  );

INTN
GetTagCount (
  TagPtr dict
  );

EFI_STATUS
GetElement (
  TagPtr dict,
  INTN   id, 
  TagPtr *dict1
);

EFI_STATUS
SaveSettings ();

UINTN
iStrLen(
  CHAR8* String,
  UINTN  MaxLen
  );

EFI_STATUS
PrepatchSmbios ();

VOID
PatchSmbios ();

VOID
FinalizeSmbios ();

EFI_STATUS
FixOwnership ();

UINT8
*Base64Decode (
  IN      CHAR8 *EncodedData,
     OUT  UINTN *DecodedSize
  );

UINT64
TimeDiff(
  UINT64 t0,
  UINT64 t1);

VOID
SetCPUProperties ();


//
// BootOptions.c
//

/** Returns the first occurrence of a Null-terminated Unicode SearchString in a Null-terminated Unicode String. Compares just first 8 bits of chars (valid for ASCII), case insensitive. */
/*CHAR16 *
EFIAPI
StrStriBasic (
  IN CONST  CHAR16 *String,
  IN CONST  CHAR16 *SearchString
  );
*/
/** Returns 0 if two strings are equal, !=0 otherwise. Compares just first 8 bits of chars (valid for ASCII), case insensitive. */
UINTN
EFIAPI
StrCmpiBasic(
  IN  CHAR16 *String1,
  IN  CHAR16 *String2
  );

/** Finds and returns pointer to specified DevPath node in DevicePath or NULL. */
EFI_DEVICE_PATH_PROTOCOL *
FindDevicePathNodeWithType (
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN  UINT8                    Type,
  IN  UINT8                    SubType      OPTIONAL
  );

//Parses BootXXXX (XXXX = BootNum) var (from BootOption->Variable) and returns it in BootOption.
EFI_STATUS
ParseBootOption (OUT BO_BOOT_OPTION  *BootOption);

/** Prints BootXXXX vars found listed in BootOrder, plus print others if AllBootOptions == TRUE. */
VOID
PrintBootOptions (
  IN  BOOLEAN AllBootOptions
  );

/** Prints BootOrder with DBG. */
VOID
PrintBootOrder (
    IN  UINT16 BootOrder[],
    IN  UINTN  BootOrderLen
                );

/** Reads BootXXXX (XXXX = BootNum) var, parses it and returns in BootOption.
 *  Caller is responsible for releasing BootOption->Variable with FreePool().
 */
EFI_STATUS
GetBootOption (
  IN      UINT16         BootNum,
     OUT  BO_BOOT_OPTION *BootOption
  );

/** Returns gEfiGlobalVariableGuid:BootOrder as UINT16 array and it's length (num of elements).
 *  Caller is responsible for releasing BootOrder mem (FreePool()).
 */
EFI_STATUS
GetBootOrder (
  OUT  UINT16 *BootOrder[],
  OUT  UINTN  *BootOrderLen
  );

/** Searches BootXXXX vars for entry that points to given FileDeviceHandle/FileName
 *  and returns BootNum (XXXX in BootXXXX variable name) and BootIndex (index in BootOrder)
 *  if found.
 */
EFI_STATUS
FindBootOptionForFile (
       IN      EFI_HANDLE FileDeviceHandle,
       IN      CHAR16     *FileName,
       OUT     UINT16     *BootNum,
       OUT     UINTN      *BootIndex
    );

/** Adds new boot option for given file system device handle FileDeviceHandle, file path FileName
 *  and Description, to be BootIndex in the list of options (0 based).
 *  If UseShortForm == TRUE, then only the hard drive media dev path will be used instead
 *  of full device path.
 *  Long (full) form:
 *   PciRoot(0x0)/Pci(0x1f,0x2)/Sata(0x1,0x0)/HD(1,GPT,96004846-a018-49ad-bc9f-4e5a340adc4b,0x800,0x64000)/\EFI\BOOT\File.efi
 *  Short form:
 *   HD(1,GPT,96004846-a018-49ad-bc9f-4e5a340adc4b,0x800,0x64000)/\EFI\BOOT\File.efi
 */
EFI_STATUS
AddBootOptionForFile (
                      IN  EFI_HANDLE FileDeviceHandle,
                      IN  CHAR16     *FileName,
                      IN  BOOLEAN    UseShortForm,
                      IN  CHAR16     *Description,
                      IN  UINT8      *OptionalData,
                      IN  UINTN      OptionalDataSize,
                      IN  UINTN      BootIndex,
                      OUT UINT16     *BootNum
                      );

/** Deletes boot option specified with BootNum (XXXX in BootXXXX var name). */
EFI_STATUS
DeleteBootOption (
  IN  UINT16 BootNum
  );


/** Deletes boot option for file specified with FileDeviceHandle and FileName. */
EFI_STATUS
DeleteBootOptionForFile (
  IN  EFI_HANDLE FileDeviceHandle,
  IN  CHAR16     *FileName
  );

/** Deletes all boot option that points to a file which contains FileName in it's path. */
EFI_STATUS
DeleteBootOptionsContainingFile (
  IN  CHAR16 *FileName
  );

//get default boot 
VOID GetBootFromOption(VOID);
//
// check if this entry corresponds to Boot# variable and then set BootCurrent
//
VOID
SetBootCurrent(REFIT_MENU_ENTRY *LoadedEntry);

//
// PlatformDriverOverride.c
//
/** Registers given PriorityDrivers (NULL terminated) to highest priority during connecting controllers.
 *  Does this by installing our EFI_PLATFORM_DRIVER_OVERRIDE_PROTOCOL
 *  or by overriding existing EFI_PLATFORM_DRIVER_OVERRIDE_PROTOCOL.GetDriver.
 */
VOID
RegisterDriversToHighestPriority (
  IN  EFI_HANDLE *PriorityDrivers
  );

EFI_STATUS
LoadUserSettings (
  IN  EFI_FILE *RootDir,
      CHAR16   *ConfName,
      TagPtr   *dict
  );

VOID
ParseSMBIOSSettings (
  TagPtr dictPointer
  );



//
// Hibernate.c
//
/** Returns TRUE if given OSX on given volume is hibernated
 *  (/private/var/vm/sleepimage exists and it's modification time is close to volume modification time).
 */
BOOLEAN
IsOsxHibernated (
  IN REFIT_VOLUME *Volume
  );

/** Prepares nvram vars needed for boot.efi to wake from hibernation. */
BOOLEAN
PrepareHibernation (
  IN REFIT_VOLUME *Volume
  );

//
// entry_scan
//
INTN
StrniCmp (
  IN CHAR16 *Str1,
  IN CHAR16 *Str2,
  IN UINTN  Count
  );

CHAR16
*StriStr(
  IN CHAR16 *Str,
  IN CHAR16 *SearchFor
  );

VOID
StrToLower (
  IN CHAR16 *Str
  );

VOID
AlertMessage (
  IN CHAR16 *Title,
  IN CHAR16 *Message
  );

BOOLEAN
YesNoMessage (
  IN CHAR16 *Title,
  IN CHAR16 *Message);


#endif
