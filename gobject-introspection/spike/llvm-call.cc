// This is based on examples/HowToUseJIT/HowToUseJIT.cpp in LLVM source archive.
// This file is distributed under the University of Illinois Open Source
// License. See http://releases.llvm.org/6.0.1/LICENSE.TXT for details.

#include <stdio.h>

#include <memory>

#include <llvm/Support/TargetSelect.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>

int
main(void)
{
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();

  llvm::LLVMContext context;

  auto owner = std::make_unique<llvm::Module>("test", context);
  auto module = owner.get();

  auto add =
    static_cast<llvm::Function *>(
      module->getOrInsertFunction("add",
                                  llvm::Type::getInt32Ty(context),
                                  llvm::Type::getInt32Ty(context),
                                  llvm::Type::getInt32Ty(context)));
  add->deleteBody();
  auto call_add =
    static_cast<llvm::Function *>(
      module->getOrInsertFunction("call_add",
                                  llvm::Type::getInt32Ty(context)));
  auto block = llvm::BasicBlock::Create(context, "EntryBlock", call_add);
  llvm::IRBuilder<> builder(block);

  auto one = builder.getInt32(1);
  auto two = builder.getInt32(2);
  auto call_instruction = builder.CreateCall(add, {one, two});
  builder.CreateRet(call_instruction);

  auto engine = llvm::EngineBuilder(std::move(owner)).create();

  llvm::outs() << "We just constructed this LLVM module:\n\n" << *module;
  llvm::outs() << "\n\nRunning foo: ";
  llvm::outs().flush();

  std::vector<llvm::GenericValue> no_args;
  auto added = engine->runFunction(call_add, no_args);
  llvm::outs() << "Result: " << added.IntVal << "\n";
  delete engine;

  llvm::llvm_shutdown();

  return 0;
}
