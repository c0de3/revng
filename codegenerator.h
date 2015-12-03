#ifndef _CODEGENERATOR_H
#define _CODEGENERATOR_H

// Standard includes
#include <cstdint>
#include <string>
#include <memory>

// LLVM includes
#include "llvm/ADT/ArrayRef.h"

// Local includes
#include "revamb.h"

// Forward declarations
namespace llvm {
class LLVMContext;
class Function;
class Module;
class Value;
class StructType;
class DataLayout;
};

class DebugHelper;

/// Translates code from an architecture to LLVM IR.
class CodeGenerator {
public:
  /// Create a new code generator translating code from an architecture to
  /// another, writing the corresponding LLVM IR and debug source file of the
  /// requested type to file.
  ///
  /// \param Source source architecture.
  /// \param Target target architecture.
  /// \param OutputPath path where the generate LLVM IR must be saved.
  /// \param HelpersPath path of the LLVM IR file containing the QEMU helpers.
  /// \param DebugInfo type of debug information to generate.
  /// \param DebugPath path where the debugging source file must be written.
  CodeGenerator(Architecture& Source,
                Architecture& Target,
                std::string Output,
                std::string Helpers,
                DebugInfoType DebugInfo,
                std::string Debug);

  ~CodeGenerator();

  /// \brief Creates an LLVM function for the code in the specified memory area.
  /// If debug information have been requested, the debug source files will be
  /// create in this phase.
  ///
  /// \param Name the name to give to the newly created function.
  /// \param Code reference to memory area containing the code to translate.
  void translate(size_t LoadAddress,
                 llvm::ArrayRef<uint8_t> Code,
                 size_t VirtualAddress,
                 std::string Name);

  /// Serialize the generated LLVM IR to the specified output path.
  void serialize();

private:
  Architecture& SourceArchitecture;
  Architecture& TargetArchitecture;
  llvm::LLVMContext& Context;
  std::unique_ptr<llvm::Module> TheModule;
  std::unique_ptr<llvm::Module> HelpersModule;
  std::string OutputPath;
  std::unique_ptr<DebugHelper> Debug;
  llvm::StructType *CPUStateType;
  const llvm::DataLayout *HelpersModuleLayout;

  unsigned OriginalInstrMDKind;
  unsigned PTCInstrMDKind;
  unsigned DbgMDKind;
};

#endif // _CODEGENERATOR_H