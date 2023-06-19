#include "headers/codegen.h"


namespace Codegen
{
    #define InternalInfo struct INTERNAL_INFO
    const bool DEBUG = true;

    struct INTERNAL_INFO
    {
        size_t temp_count;
        // llvm::IRBuilder<> builder;
        llvm::Function* function;
        std::vector<llvm::Value*> variable_values;
        llvm::Module* module;
        llvm::LLVMContext* context;
    };

    bool isValidInfo(InternalInfo *info)
    {
        return (
            info != NULL &&
            info->function != NULL &&
            info->module != NULL
        );
    }

    void generate_function_body(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    llvm::Value* generate_binary_expression(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    llvm::Value* generate_expression(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    void generate_return(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    void generate_variable_declaration(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    llvm::Value* generate_variable_assignment(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    llvm::Value* generate_increment(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    llvm::Value* generate_decrement(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    llvm::Value* generate_multiply_assign(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    llvm::Value* generate_divide_assign(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    llvm::Value* generate_function_call(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    llvm::BasicBlock* generate_for(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    void generate_if(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder);
    void generate_function(AST *ast, llvm::Module* module, llvm::IRBuilder<> builder, llvm::LLVMContext* context);

    void codegen_generate(Codegen *codegen, llvm::Module* module, llvm::IRBuilder<> builder, llvm::LLVMContext* context);

    void init(AST *ast)
    {
        // auto test = llvm::registerPass();
        llvm::PassRegistry &Registry = *llvm::PassRegistry::getPassRegistry();
        llvm::initializeCore(Registry);
        // LLVMInitializeCore(LLVMGetGlobalPassRegistry());
        llvm::InitializeNativeTarget();
        // LLVMInitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        // LLVMInitializeNativeAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();
        llvm::initializeWriteBitcodePassPass(*llvm::PassRegistry::getPassRegistry());
        // llvm::initializeTarget(*llvm::PassRegistry::getPassRegistry("x86_64-pc-linux-gnu"));
        // LLVMInitializeNativeAsmParser();

        std::cout << "stage 1" << std::endl;

        // LLVMModuleRef module = LLVMModuleCreateWithName("new_module");
        llvm::LLVMContext* context = new llvm::LLVMContext();
        llvm::Module *module = new llvm::Module("new_module", *context);
        module->setTargetTriple("x86_64-pc-linux-gnu");
        // module->setTargetTriple("x86_64-pc-win32");
        // module_test = module_test; // DEBUG
        // auto module 
        // char* target = LLVMGetDefaultTargetTriple();
        llvm::InitializeAllTargetInfos();
        // LLVMSetTarget(module, target); // saet target triple to x86_64-pc-linux-gnu
        // free(target);
        // auto builder = llvm::createBuilder();

        std::cout << "stage 2" << std::endl;

        // llvm::IRBuilder<> builder = llvm::IRBuilder<>();
        llvm::IRBuilder<> builder(*context);

        // LLVMBuilderRef builder = LLVMCreateBuilder(); // can be removed

        // llvm::Type *int_type = llvm::Type::getInt32Ty(*context);

        // LLVMTypeRef printf_args[] = {LLVMPointerType(LLVMInt8Type(), 0)};

        llvm::Type *printf_args_type[] = {llvm::PointerType::get(llvm::IntegerType::get(*context, 8), 0)};

        llvm::ArrayRef<llvm::Type*> printf_args_type_ref(printf_args_type);

        // llvm::addFunctionAttr(printf_args_type[0], llvm::Attribute::NoCapture);

        llvm::FunctionType* printf_type = llvm::FunctionType::get(llvm::IntegerType::get(*context, 32), printf_args_type_ref, false);

        // module->functions().add(llvm::Function::Create(llvm::FunctionType::get(llvm::IntegerType::get(*context, 32), printf_args_type, false), llvm::Function::ExternalLinkage, "puts", module));

        llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, "puts", module);

        // LLVMAddFunction(module, "puts", LLVMFunctionType(LLVMInt32Type(), printf_args, 1, 0));
        
        std::cout << "stage 3" << std::endl;

        // Codegen *codegen = (Codegen*) malloc(sizeof(Codegen));
        Codegen *codegen = new Codegen(ast);

        codegen_generate(codegen, module, builder, context);


        // Working
        /*
        llvm::Function* main_function = llvm::Function::Create(llvm::FunctionType::get(llvm::IntegerType::get(*context, 32), false), llvm::Function::ExternalLinkage, "main", module);
        main_function->setCallingConv(llvm::CallingConv::C);
        // main_function->addFnAttr(llvm::Attribute::NoUnwind);
        // llvm::BasicBlock* main_block = llvm::BasicBlock::Create(*context, "main_entry", main_function);
        llvm::BasicBlock* main_block = llvm::BasicBlock::Create(*context, "main_entry", main_function);
        llvm::IRBuilder<> main_builder(main_block);
        // builder.CreateRet(llvm::ConstantInt::get(llvm::IntegerType::get(*context, 32), 0, true));
        main_builder.CreateAlloca(llvm::IntegerType::get(*context, 32), 0, "main_alloca");
        // llvm::ReturnInst::Create(*context, llvm::ConstantInt::get(llvm::IntegerType::get(*context, 32), 0, true), main_block);
        llvm::Value* val = main_builder.CreateAdd(llvm::ConstantInt::get(llvm::IntegerType::get(*context, 32), 1, true), llvm::ConstantInt::get(llvm::IntegerType::get(*context, 32), 2, true), "main_add");
        main_builder.CreateRet(val);
        */
        std::cout << "stage 4" << std::endl;
        // LLVMWriteBitcodeToFile(module, "main.bc");
        // llvm::WriteBitcodeToFile(*module, "main.bc");
        // std::cout << "verifying module" << std::endl;
        // bool test = llvm::verifyModule(*module);
        // if(!test)
        // {
        //     std::cout << "module is not valid" << std::endl;
        //     return;
        // }
        std::cout << "creating filename stringref" << std::endl;
        llvm::StringRef filename("main.ll");
        std::cout << "creating error_code" << std::endl;
        std::error_code error_code;
        std::cout << "creating raw_fd_ostream" << std::endl;
        llvm::raw_fd_ostream out(filename, error_code);
        std::cout << "checking error_code" << std::endl;
        if(error_code.value() != 0)
        {
            std::cout << "error_code is not 0 (" << error_code << ")" << std::endl;
            return;
        }
        std::cout << "error_code is " << error_code << std::endl;
        // std::cout << "writing bitcode to file" << std::endl;
        // llvm::WriteBitcodeToFile(*module, out);
        // std::cout << "writing module to file" << std::endl;
        // std::cout << "creating buffer" << std::endl;
        // llvm::SmallVectorImpl<char> &Buffer = *new llvm::SmallVector<char, 0>();
        // std::cout << "creating bitcode_writer" << std::endl;
        // llvm::BitcodeWriter bitcode_writer = llvm::BitcodeWriter(Buffer);
        // std::cout << "writing module" << std::endl;
        // bitcode_writer.writeModule(*module);
        // std::cout << "writing buffer" << std::endl;
        // out << Buffer;

        // Working
        // /*
        std::error_code EC;
        llvm::raw_fd_ostream OS("main.bc", EC, llvm::sys::fs::F_None);
        WriteBitcodeToFile(*module, OS);
        OS.flush();
        // */

      //  module->print(out, nullptr); // used to print module to file in human readable format


        // std::cout << module->getValueSymbolTable() << std::endl;
        // module->print(out, nullptr);
        // if(DEBUG) LLVMPrintModuleToFile(module, "main.ll", NULL);
        // Free(codegen);
        std::cout << "stage 5" << std::endl;
        delete codegen;
        // LLVMDisposeBuilder(builder);
        // LLVMDisposeModule(module);
        // llvm::disposeM
        // llvm::DisposeBuilder(builder);
    }
    void codegen_generate(Codegen *codegen, llvm::Module* module, llvm::IRBuilder<> builder, llvm::LLVMContext* context)
    {
        if(codegen == NULL){printf("Codegen is NULL\n");return;}
        if(codegen->ast == NULL){printf("AST is NULL\n");return;}
        if(module == NULL){printf("Module is NULL\n");return;}
        AST *ast = codegen->ast;
        std::cout << "generate start" << std::endl;
        for(size_t i = 0; i < ast->children.size(); i++)
        {
            AST *child = ast->children[i];
            switch (child->tag)
            {
            case Ast::FUNCTION:
                generate_function(child, /*file, */module, builder, context);
                break;
            default:
                printf("Unknown tag: %d\n", child->tag);
                break;
            }
        }
        std::cout << "generate start" << std::endl;
    }
    bool file_and_ast_valid(FILE *file, AST *ast)
    {
        return file != NULL && ast != NULL;
    }
    llvm::Type* get_type(AST *ast, llvm::LLVMContext* context)
    {
        if(ast == NULL){printf("AST is NULL\n");return NULL;}
        char* type_name = ast->data.TYPE.name;
        if(strcmp(type_name, "int") == 0)
        {
            std::cout << "get_type int" << std::endl;
            return llvm::IntegerType::get(*context, 32);
        }
        if(strcmp(type_name, "float") == 0)
        {
            std::cout << "get_type float" << std::endl;
            return llvm::Type::getDoubleTy(*context);
        }
        else if(strcmp(type_name, "void") == 0)
        {
            std::cout << "get_type void" << std::endl;
            return llvm::Type::getVoidTy(*context);
        }
        else
        {
            printf("Unknown type: %s\n", type_name);
            return NULL;
        }
    }
    void generate_function(AST *ast, llvm::Module* module, llvm::IRBuilder<> b, llvm::LLVMContext* context)
    {
        std::cout << "generate_function start" << std::endl;
        // TODO: breaking in this function
        if(ast == NULL){printf("AST is NULL\n");return;}
        if(module == NULL){printf("Module is NULL\n");return;}
        char* function_name = ast->data.FUNCTION.name;
        // unsigned int param_count = ast->arguments.size();
        // LLVMTypeRef *param_types = (LLVMTypeRef*) malloc(sizeof(LLVMTypeRef) * param_count);
        std::vector<llvm::Type*> param_types;
        for(AST* child : ast->arguments)
        {
            AST *type = child->data.VAR_MANIP.ast;
            llvm::Type* type_ref = get_type(type, context);
            // llvm::Type *type_ref = llvm::IntegerType::get(*context, 32); // TODO: change to get_type and change get_type to return llvm::Type*
            if(type_ref == NULL){printf("Type is NULL\n");return;}
            param_types.push_back(type_ref);
        }
        // {
        //     AST *child = ast->arguments[i];
        //     AST *type = child->data.VAR_MANIP.ast;
        //     LLVMTypeRef type_ref = get_type(type);
        //     if(type_ref == NULL){printf("Type is NULL\n");return;}
        //     param_types[i] = type_ref;
        // }
        // LLVMTypeRef function_type = LLVMFunctionType(LLVMInt32Type(), param_types, param_count, false);
        // free(param_types);
        llvm::FunctionType* function_type = llvm::FunctionType::get(llvm::IntegerType::get(*context, 32), param_types, false); // TODO: change to adjust to function specific type
        // LLVMValueRef function = LLVMAddFunction(module, function_name, function_type);
        // llvm::Function::Create(function_type, llvm::GlobalValue::LinkageTypes::, function_name, module);
        // llvm::Function module->getOrInsertFunction(function_name, function_type); // HELP: https://releases.llvm.org/2.6/docs/tutorial/JITTutorial1.html
        llvm::Function* function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, function_name, module);
        function->setCallingConv(llvm::CallingConv::C);
        std::string function_name_str(function_name);
        llvm::BasicBlock* entry = llvm::BasicBlock::Create(*context, function_name_str, function);
        // function->getBasicBlockList().push_back(entry);
        llvm::IRBuilder<> builder(entry);
        // llvm::IRBuilder<> builder(entry);
        // llvm::Function* function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, function_name, module);
        // llvm::Function::Create(llvm::FunctionType::get(llvm::IntegerType::get(*context, 32), printf_args_type_ref, false), llvm::Function::ExternalLinkage, "puts", module);
        // LLVMSetLinkage(function, LLVMExternalLinkage);
        // LLVMSetFunctionCallConv(function, LLVMCCallConv);
        // LLVMBasicBlockRef entry = LLVMAppendBasicBlock(function, "entry");
        // LLVMPositionBuilderAtEnd(builder, entry);
        InternalInfo *info = (InternalInfo*) malloc(sizeof(InternalInfo));
        // InternalInfo *info = new InternalInfo(); // POSSIBLE CRASH
        // info->builder = builder;
        info->temp_count = 0;
        // info->variables = NULL;
        // info->variable_count = 0;
        info->function = function;
        info->module = module;
        info->context = context;
        // llvm::BasicBlock::Create(*context, "entry", function);
        // function->addBasicBlock(entry);
        // llvm::BasicBlock* entry_ = llvm::BasicBlock::Create(*context, "entry1", function);
        // llvm::IRBuilder<> b1 = llvm::IRBuilder<>(entry_);
        // /*auto* A = */b1.CreateAlloca (llvm::Type::getInt32Ty(*context), nullptr, "a");
        generate_function_body(ast->data.FUNCTION.body, info, builder);
        std::cout << "generate_function end" << std::endl;
        // free(info->variables);
        free(info);
    }
    llvm::BasicBlock* generate_body_content(AST* child, InternalInfo* info, llvm::IRBuilder<> builder)
    {
      llvm::BasicBlock* block = NULL;
      switch (child->tag)
      {
      case Ast::RETURN:
            generate_return(child, info, builder);
            break;
      case Ast::DECLARATION:
            generate_variable_declaration(child, info, builder);
            break;
      case Ast::ASSIGN:
            generate_variable_assignment(child, info, builder);
            break;
      case Ast::CALL:
            printf("Call begin\n");
            generate_function_call(child, info, builder);
            printf("Call end\n");
            break;
      case Ast::INCREMENT:
            generate_increment(child, info, builder);
            break;
      case Ast::DECREMENT:
            generate_decrement(child, info, builder);
            break;
      case Ast::MULTIPLY_ASSIGN:
            generate_multiply_assign(child, info, builder);
            break;
      case Ast::DIVIDE_ASSIGN:
            generate_divide_assign(child, info, builder);
            break;
      case Ast::FOR:
            block = generate_for(child, info, builder);
            break;
      case Ast::IF:
            generate_if(child, info, builder);
            break;
      default:
            printf("Unknown tag: %s\n", Ast::get_tag_name(child->tag).c_str());
            break;
      }
      return block;
    }
    void generate_function_body(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder)
    {
        std::cout << "generate_function_body start" << std::endl;
        if(ast == NULL){printf("AST is NULL\n");return;}
        // if(info->variables != NULL) free(info->variables);
        // info->variables = (LLVMValueRef*) calloc(0, sizeof(LLVMValueRef));
        info->variable_values.clear();
        if(!isValidInfo(info)){printf("Info is invalid (function_body)\n");return;}
        if(ast->tag != Ast::NODE) return;
        if(ast->children.size() == 0) return;

        info->temp_count = 0;
        // info->variable_count = 0;

        for(size_t i = 0; i < ast->children.size(); i++)
        {
            AST *child = ast->children[i];
            llvm::BasicBlock* block = generate_body_content(child, info, builder);
            if(block != NULL)
            {
                builder.SetInsertPoint(block);
            }
        }
        std::cout << "generate_function_body end" << std::endl;
    }
    llvm::Value* get_number_type(AST *ast, llvm::LLVMContext* context)
    {
        if(ast == NULL) return NULL;
        char* literal = ast->data.LITERAL.literal;
        float f;
        (void)sscanf(literal, "%f", &f);
        // LLVMValueRef value = NULL;
        llvm::Value* value = NULL;
        bool is_float = false;
        for(size_t i = 0; i < strlen(literal); i++)
        {
            if(literal[i] == '.')
            {
                is_float = true;
                break;
            }
        }

        if(is_float)
        {
            printf("----------\nnumber: %f\n", atof(literal));
            // value = LLVMConstReal(LLVMDoubleType(), atof(literal));
            value = llvm::ConstantFP::get(*context, llvm::APFloat(atof(literal)));
            return value;
        } else {
            printf("literal: %s\n", literal);
            // value = LLVMConstInt(LLVMInt32Type(), atoi(literal), 0);
            value = llvm::ConstantInt::get(*context, llvm::APInt(32, atoi(literal), true));
        }
        return value;
    }
    bool is_std_function(char *name)
    {
        if(strcmp(name, "print") == 0) return true;
        return false;
    }
    llvm::Type* get_variable_type(char* name, InternalInfo *info)
    {
        if(!isValidInfo(info)){printf("Info is invalid (variable_type)\n");return NULL;}
        // for(size_t i = 0; i < info->variable_count; i++)
        // {
        //     if(strcmp(name, LLVMGetValueName(info->variables[i])) == 0)
        //     {
        //         return info->variables[i];
        //     }
        // }
        for(llvm::Value* variable : info->variable_values)
        {
            if(variable->getName() == name)
            {
                return variable->getType();
            }
        }
        return NULL;
    }
    llvm::Value* generate_std_function_call(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder)
    {
        if(!isValidInfo(info)){printf("Info is invalid(std function call)\n");return NULL;}
        if(ast == NULL){printf("AST is NULL\n");return NULL;}
        if(ast->tag != Ast::CALL){printf("AST is not a call\n");return NULL;}
        char *function_name = ast->data.CALL.name;
        if(strcmp(function_name, "print") == 0)
        {
            if(ast->arguments.size() != 1){printf("Wrong literal of arguments for print\n");return NULL;} // could break
            AST *argument = ast->arguments[0];
            // LLVMValueRef value = generate_expression(argument, info);
            llvm::Value* value = generate_expression(argument, info, builder);
            if(value == NULL){printf("Value is NULL\n");return NULL;}
            // LLVMTypeRef type = LLVMTypeOf(value);
            llvm::Type* type = value->getType();
            // llvm::StringRef type_name = type->getStructName();
            llvm::Type::TypeID type_id = type->getTypeID();
            // char* type_name = LLVMPrintTypeToString(type);
            if(type == NULL){printf("Type is NULL\n");return NULL;}
            // LLVMValueRef function = LLVMGetNamedFunction(info->module, "printf");
            llvm::Function* function = info->module->getFunction("printf");
            if(function == NULL)
            {
                // LLVMTypeRef *printf_args = (LLVMTypeRef*) calloc(1, sizeof(LLVMTypeRef));
                std::vector<llvm::Type*> printf_args;
                // printf_args[0] = LLVMPointerType(LLVMInt8Type(), 0);
                printf_args.push_back(llvm::Type::getInt8PtrTy(*info->context));
                llvm::FunctionType* printf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(*info->context), printf_args, true);
                // LLVMTypeRef printf_type = LLVMFunctionType(LLVMInt32Type(), printf_args, 1, true);
                // free(printf_args);
                // function = LLVMAddFunction(info->module, "printf", printf_type);
                function = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, "printf", info->module);
                // LLVMSetLinkage(function, LLVMExternalLinkage);
                function->setLinkage(llvm::Function::ExternalLinkage);
                function->setCallingConv(llvm::CallingConv::C);
                // LLVMSetFunctionCallConv(function, LLVMCCallConv);
            }

            llvm::Value* ret = NULL;
            if(type_id == llvm::Type::IntegerTyID)
            {
                // LLVMValueRef format_str = LLVMBuildGlobalStringPtr(info->builder, "%d\n", "format_str_d");
                // LLVMValueRef args[2] = {format_str, value};
                // ret = LLVMBuildCall(info->builder, function, args, 2, "ret");
                llvm::Value* format_str = builder.CreateGlobalStringPtr("%d\n", "format_str_d");
                std::vector<llvm::Value*> args;
                args.push_back(format_str);
                args.push_back(value);
                ret = builder.CreateCall(function, args, "ret");
            } else if(type_id == llvm::Type::DoubleTyID)
            {
                // LLVMValueRef format_str = LLVMBuildGlobalStringPtr(info->builder, "%f\n", "format_str_f");
                // LLVMValueRef args[2] = {format_str, value};
                // ret = LLVMBuildCall(info->builder, function, args, 2, "ret");
                llvm::Value* format_str = builder.CreateGlobalStringPtr("%f\n", "format_str_f");
                std::vector<llvm::Value*> args;
                args.push_back(format_str);
                args.push_back(value);
                ret = builder.CreateCall(function, args, "ret");
            }
            if(ret == NULL) printf("Unknown type for print\n");
            // free(type_name);
            return ret;
        }
        printf("Unknown std function: %s\n", function_name);
        return NULL;
    }
    llvm::Value* generate_function_call(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder)
    {
        std::cout << "generate_function_call" << std::endl;
        if(!isValidInfo(info)){printf("Info is invalid(function call)\n");return NULL;}
        if(ast == NULL){printf("AST is NULL\n");return NULL;}
        if(ast->tag != Ast::CALL){printf("AST is not a call\n");return NULL;}
        char *function_name = ast->data.CALL.name;
        if(is_std_function(function_name)) return generate_std_function_call(ast, info, builder);
        size_t argc = ast->arguments.size();
        // LLVMValueRef function = LLVMGetNamedFunction(info->module, function_name);
        llvm::Function* function = info->module->getFunction(function_name);
        // LLVMValueRef* args = (LLVMValueRef*) calloc(argc, sizeof(LLVMValueRef));
        std::vector<llvm::Value*> args;
        for(size_t i = 0; i < argc; i++)
        {
            // args[i] = generate_expression(ast->arguments[i], info, builder);
            llvm::Value* arg = generate_expression(ast->arguments[i], info, builder);
            args.push_back(arg);
            if(args[i] == NULL) return NULL;
        }
        char* temp_name = (char*) calloc(10, sizeof(char));
        sprintf(temp_name, "_%ld", info->temp_count++);
        temp_name = (char*) realloc(temp_name, sizeof(char) * (strlen(temp_name) + 1));
        // LLVMValueRef result = LLVMBuildCall(info->builder, function, args, argc, temp_name);
        llvm::Value* result = builder.CreateCall(function, args, temp_name);
        free(temp_name);
        // free(args);
        std::cout << "generate_function_call end" << std::endl;
        return result;
    }
    llvm::Value* generate_variable_assignment(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder)
    {
        std::cout << "Generating variable assignment" << std::endl;
        if(ast == NULL){printf("AST is NULL\n");return NULL;}
        if(!isValidInfo(info)){printf("Info is invalid(variable assignment)\n");return NULL;}
        if(ast->tag != Ast::ASSIGN){printf("AST is not an assignment\n");return NULL;}
        char* name = ast->data.VAR_MANIP.name;
        AST *expr = ast->data.VAR_MANIP.ast;
        llvm::Value* value = generate_expression(expr, info, builder);
        if(value == NULL) return NULL;
        // LLVMValueRef variable = NULL;
        // llvm::Value* variable = NULL;
        // for(size_t i = 0; i < info->variable_count; i++)
        // {
        //     if(strcmp(name, LLVMGetValueName(info->variables[i])) == 0)
        //     {
        //         variable = info->variables[i];
        //         break;
        //     }
        // }
        // std::cout << "searching for variable: " << name << std::endl;
        // for(llvm::Value* variable : info->variable_values)
        // {
        //     std::cout << "Variable name: " << variable->getName().str() << std::endl;
        //     if(variable->getName() == name)
        //     {
        //         std::cout << "Found variable" << std::endl;
        //         variable = variable;
        //         break;
        //     }
        // }
        // LLVMBuildStore(info->builder, value, variable);
        // info->module->getNamedValue(name);
        // builder.CreateStore(value, variable);
        std::string str = "%";
        str += name;
        // llvm::Value* variable = info->module->getNamedValue(str);
        for(llvm::Value* variable : info->variable_values)
        {
            std::string str = variable->getName().str();
            std::cout << "Variable name: " << str << " searched: " << name << "|" << (str == name) << std::endl;
            if(str == name)
            {
                std::cout << "Found variable!!!" << std::endl;
                if(variable == NULL) std::cout << "Variable is NULL" << std::endl;
                builder.CreateStore(value, variable);
                return value;
            }
        }
        std::cout << "Variable not found, creating new variable" << std::endl;
        // if(variable == NULL)
        // {
        //     std::cout << "Variable not found, creating new variable" << std::endl;
        //     // variable = builder.CreateAlloca(type, 0, name);
        //     // info->variable_values.push_back(variable);
        // }
            // llvm::Type* type = llvm::Type::getInt32Ty(*info->context);
            // llvm::Value* var = builder.CreateAlloca(type, 0, name);
        // builder.CreateStore(value, variable);
        std::cout << "Finished generating variable assignment" << std::endl;
        return value;
    }
    void generate_variable_declaration(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder)
    {
        std::cout << "Generating variable declaration" << std::endl;
        
        // // std::string blockname = "entry" + std::to_string(info->function->getBasicBlockList().size());
        // char* blockname = (char*) calloc(10, sizeof(char));
        // sprintf(blockname, "entry%ld", info->function->getBasicBlockList().size());
        // blockname = (char*) realloc(blockname, sizeof(char) * (strlen(blockname) + 1));
        // llvm::BasicBlock* block = llvm::BasicBlock::Create(*info->context, blockname, info->function);
        // llvm::IRBuilder<> builder(block);
        // free(blockname);

        size_t count = 0;
        if(ast == NULL){printf("AST is NULL\n");return;}
        if(!isValidInfo(info)){printf("Info is invalid(variable_declaration)\n");return;}
        if(ast->tag != Ast::DECLARATION){printf("AST is not a declaration\n");return;}
        llvm::Type* type = get_type(ast->data.VAR_MANIP.ast, info->context);
        std::cout << count++ << std::endl;
        // LLVMValueRef value = LLVMBuildAlloca(info->builder, type, ast->data.VAR_MANIP.name);
        // TODO: Add initializer possibly
        // llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*info->context), 0);
        std::cout << count++ << std::endl;
        std::string name = ast->data.VAR_MANIP.name;
        // std::string struct_name = type->getStructName();
        // std::cout << struct_name << std::endl;
        llvm::Value* value = builder.CreateAlloca(type, nullptr, name);
        std::cout << count++ << std::endl;
        // LLVMSetInitializer(value, LLVMConstInt(LLVMInt32Type(), 0, 0));
        // value->setInitializer(llvm::ConstantInt::get(llvm::Type::getInt32Ty(info->context), 0));
        info->variable_values.push_back(value);
        std::cout << count++ << std::endl;
        // info->variable_count++;
        // info->variables = (LLVMValueRef*) realloc(info->variables, sizeof(LLVMValueRef) * info->variable_count);
        // info->variables[info->variable_count - 1] = value;
        std::cout << "Finished generating variable declaration" << std::endl;
    }
    void generate_return(AST *ast, InternalInfo* info, llvm::IRBuilder<> builder)
    {
        std::cout << "Generating return start" << std::endl;
        if(ast == NULL) return;
        if(ast->data.RETURN.expr == NULL) return;
        if(!isValidInfo(info)) return;
        AST *expr = ast->data.RETURN.expr;
        llvm::Value *value = generate_expression(expr, info, builder);
        // LLVMValueRef value = generate_expression(expr, info);
        if(value == NULL) return;
        llvm::BasicBlock* block = builder.GetInsertBlock();
        llvm::LLVMContext& context = block->getContext();
        llvm::ReturnInst::Create(context, value, block);
        // LLVMBuildRet(info->builder, value);
        std::cout << "Generating return end" << std::endl;
    }
    llvm::Value* generate_expression(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder)
    {
        std::cout << "Generating expression start" << std::endl;
        if(ast == NULL) return NULL;
        if(!isValidInfo(info)) return NULL;
        char* temp_name = (char*) calloc(10, sizeof(char)); // some leak is happening with this
        // LLVMValueRef value = NULL;
        llvm::Value* value = NULL;
        switch (ast->tag)
        {
            case Ast::LITERAL:
                value = get_number_type(ast, info->context);
                break;
            case Ast::VARIABLE:
                if(ast->data.VARIABLE.is_arg)
                {
                    // return LLVMGetParam(info->function, ast->data.VARIABLE.arg_index);
                    std::cout << "Critical point begin" << std::endl;
                    return info->function->arg_begin() + ast->data.VARIABLE.arg_index; // INFO: This could be wrong
                    std::cout << "Critical point begin" << std::endl;
                }
                sprintf(temp_name, "_%ld", info->temp_count++);
                temp_name = (char*) realloc(temp_name, sizeof(char) * (strlen(temp_name) + 1));
                value = NULL;
                // for(size_t i = 0; i < info->variable_count; i++)
                // {
                //     if(strcmp(ast->data.VARIABLE.name, LLVMGetValueName(info->variables[i])) == 0)
                //     {
                //         value = info->variables[i];
                //     }
                // }
                for(llvm::Value* variable : info->variable_values)
                {
                    if(variable->getName() == ast->data.VARIABLE.name)
                    {
                        value = variable;
                    }
                }
                if(value == NULL) return NULL;
                // value = LLVMBuildLoad(info->builder, value, temp_name);
                value = builder.CreateLoad(value, temp_name);
                free(temp_name);
                std::cout << "Generating expression end" << std::endl;
                return value;
            case Ast::BINARY_OP:
                value = generate_binary_expression(ast, info, builder);
                break;
            case Ast::CALL:
                value = generate_function_call(ast, info, builder);
                break;
            case Ast::INCREMENT:
                value = generate_increment(ast, info, builder);
                break;
            case Ast::DECREMENT:
                value = generate_decrement(ast, info, builder);
                break;
            case Ast::MULTIPLY_ASSIGN:
                value = generate_multiply_assign(ast, info, builder);
                break;
            case Ast::DIVIDE_ASSIGN:
                value = generate_divide_assign(ast, info, builder);
                break;
            case Ast::NOOP:
                value = NULL;
                break;
            case Ast::ASSIGN:
                value = generate_variable_assignment(ast, info, builder);
                break;
            case Ast::FOR:
            case Ast::EXPR:
            case Ast::RETURN:
            case Ast::ARGUMENT:
            case Ast::DECLARATION:
            case Ast::IF:
            case Ast::WHILE:
            case Ast::BREAK:
            case Ast::CONTINUE:
            case Ast::TYPE:
            case Ast::FUNCTION:
            case Ast::IMPORT:
            case Ast::NODE:
            case Ast::UNARY_OP:
                printf("Unknown tag: %s\n", Ast::get_tag_name(ast->tag).c_str());
                exit(5);
                break;
        }
        std::cout << "Generating expression end" << std::endl;
        free(temp_name);
        return value;
    }
    llvm::Value* generate_binary_operation_with_int(char* op, llvm::Value* left, llvm::Value* right, char* temp_name, llvm::IRBuilder<> builder)
    {
        if (strcmp(op, "+") == 0)
        {
            // return LLVMBuildAdd(builder, left, right, temp_name);
            return builder.CreateAdd(left, right, temp_name);
        }
        else if (strcmp(op, "-") == 0)
        {
            // return LLVMBuildSub(builder, left, right, temp_name);
            return builder.CreateSub(left, right, temp_name);
        }
        else if (strcmp(op, "*") == 0)
        {
            // return LLVMBuildMul(builder, left, right, temp_name);
            return builder.CreateMul(left, right, temp_name);
        }
        else if (strcmp(op, "/") == 0)
        {
            // return LLVMBuildSDiv(builder, left, right, temp_name);
            return builder.CreateSDiv(left, right, temp_name);
        }
        else if (strcmp(op, "==") == 0)
        {
          return builder.CreateICmpEQ(left, right, temp_name);
        }
        else if (strcmp(op, "<") == 0)
        {
          return builder.CreateICmpULT(left, right, temp_name);
        }
        else if (strcmp(op, ">") == 0)
        {
          return builder.CreateICmpUGT(left, right, temp_name);
        }
        else if (strcmp(op, "<=") == 0)
        {
          return builder.CreateICmpULE(left, right, temp_name);
        }
        else if (strcmp(op, ">=") == 0)
        {
          return builder.CreateICmpUGE(left, right, temp_name);
        }
        else if (strcmp(op, "!=") == 0)
        {
          return builder.CreateICmpNE(left, right, temp_name);
        }
        else if (strcmp(op, "&&") == 0)
        {
          return builder.CreateAnd(left, right, temp_name);
        }
        else
        {
            printf("Unknown operator: %s\n", op);
            return NULL;
        }
    }
    llvm::Value* generate_binary_operation_with_double(char* op, llvm::Value* left, llvm::Value* right, char* temp_name, llvm::IRBuilder<> builder)
    {
        if (strcmp(op, "+") == 0)
        {
            // return LLVMBuildFAdd(builder, left, right, temp_name);
            return builder.CreateFAdd(left, right, temp_name);
        }
        else if (strcmp(op, "-") == 0)
        {
            // return LLVMBuildFSub(builder, left, right, temp_name);
            return builder.CreateFSub(left, right, temp_name);
        }
        else if (strcmp(op, "*") == 0)
        {
            // return LLVMBuildFMul(builder, left, right, temp_name);
            return builder.CreateFMul(left, right, temp_name);
        }
        else if (strcmp(op, "/") == 0)
        {
            // return LLVMBuildFDiv(builder, left, right, temp_name);
            return builder.CreateFDiv(left, right, temp_name);
        }
        else if (strcmp(op, "==") == 0)
        {
          return builder.CreateFCmpOEQ(left, right, temp_name);
        }
        else if (strcmp(op, "!=") == 0)
        {
          return builder.CreateFCmpONE(left, right, temp_name);
        }
        else if (strcmp(op, "<") == 0)
        {
          return builder.CreateFCmpOLT(left, right, temp_name);
        }
        else if (strcmp(op, ">") == 0)
        {
          return builder.CreateFCmpOGT(left, right, temp_name);
        }
        else if (strcmp(op, "<=") == 0)
        {
          return builder.CreateFCmpOLE(left, right, temp_name);
        }
        else if (strcmp(op, ">=") == 0)
        {
          return builder.CreateFCmpOGE(left, right, temp_name);
        }
        else if (strcmp(op, "&&") == 0)
        {
          return builder.CreateAnd(left, right, temp_name);
        }
        else
        {
            printf("Unknown operator: %s\n", op);
            return NULL;
        }
    }
    llvm::Value* generate_binary_operation_with_string(char* op, llvm::Value* left, llvm::Value* right, char* temp_name, llvm::IRBuilder<> builder)
    {
        llvm::Type::TypeID typekind = left->getType()->getTypeID();
        if(typekind == llvm::Type::TypeID::IntegerTyID)
        {
            return generate_binary_operation_with_int(op, left, right, temp_name, builder);
        }
        else if(typekind == llvm::Type::TypeID::DoubleTyID)
        {
            return generate_binary_operation_with_double(op, left, right, temp_name, builder);
        }
        else
        {
            printf("Unknown type: %d\n", typekind);
            return NULL;
        }
        // if(typekind == LLVMIntegerTypeKind)
        // {
        //     return generate_binary_operation_with_int(op, left, right, temp_name, builder);
        // }
        // else if(typekind == LLVMDoubleTypeKind)
        // {
        //     return generate_binary_operation_with_double(op, left, right, temp_name, builder);
        // }
        // else
        // {
        //     printf("Unknown type: %d\n", typekind);
        //     return NULL;
        // }
    }
    llvm::Value* generate_binary_expression(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder)
    {
        if(ast == NULL) return NULL;
        if(!isValidInfo(info)) return NULL;
        if(ast->data.TUPLE.op == NULL){printf("Operator is NULL\n");return NULL;}
        if(ast->data.TUPLE.left == NULL){printf("Left is NULL\n");return NULL;}
        if(ast->data.TUPLE.right == NULL){ast->data.TUPLE.left->print();printf("Right is NULL\n");return NULL;}
        char *op = ast->data.TUPLE.op;
        llvm::Value* left = generate_expression(ast->data.TUPLE.left, info, builder);
        // ast->data.TUPLE.right->print();
        llvm::Value* right = generate_expression(ast->data.TUPLE.right, info, builder);
        if(left == NULL){printf("Left is NULL\n");return NULL;}
        if(right == NULL){printf("Right is NULL (0)\n");return NULL;}
        char* temp_name = (char*) calloc(1, sizeof(char*));
        sprintf(temp_name, "_%ld", info->temp_count++);
        llvm::Value* value = generate_binary_operation_with_string(op, left, right, temp_name, builder);
        free(temp_name);
        return value;
    }
    llvm::Value *generate_increment(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder)
    {
        if (ast == NULL)
            return NULL;
        if (!isValidInfo(info))
            return NULL;
        if (ast->data.VAR_MANIP.ast == NULL)
        {
            printf("Right is NULL\n");
            return NULL;
        }
        llvm::Value *expr = generate_expression(ast->data.VAR_MANIP.ast, info, builder);
        std::cout << "Incrementing variable: " << ast->data.VAR_MANIP.name << std::endl;
        for (llvm::Value *variable : info->variable_values)
        {
            std::cout << "Variable: " << variable->getName().str() << " ast name: " << ast->data.VAR_MANIP.name << std::endl;
            if (variable->getName() == ast->data.VAR_MANIP.name)
            {
                    std::cout << "Found variable: " << variable->getName().str() << std::endl;
                    llvm::Value *value = builder.CreateLoad(variable);
                    value = builder.CreateAdd(value, expr);
                    builder.CreateStore(value, variable);
                    return value;
            }
        }
        return expr;
    }
    llvm::BasicBlock* generate_for(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder)
    {
      generate_expression(ast->data.FOR.init, info, builder);
      // generate_body_content(child, info, builder);
      llvm::BasicBlock *loop = llvm::BasicBlock::Create(*info->context, "loop", info->function);
      // AST* condition = ast->data.FOR.condition;
      // std::cout << "Condition: " << std::endl;
      // condition->print();
      builder.CreateBr(loop);
      builder.SetInsertPoint(loop);
      AST* body = ast->data.FOR.body;
      std::vector<AST*> body_children = body->children;
      for(AST* child : body_children)
      {
        generate_body_content(child, info, builder);
      }
      // end loop
      generate_body_content(ast->data.FOR.increment, info, builder);
      llvm::Value* test = generate_expression(ast->data.FOR.condition, info, builder);
      llvm::BasicBlock *after = llvm::BasicBlock::Create(*info->context, "afterloop", info->function);
      builder.CreateCondBr(test, loop, after);
      builder.SetInsertPoint(after);
      return after;
      // builder.SetInsertPoint(loop);
      // llvm::ReturnInst::Create(*info->context, builder.CreateLoad(info->variable_values[0]), after);
    }
    // TODO: Implement
    llvm::Value* generate_decrement(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder){return NULL;}
    // TODO: Implement
    llvm::Value* generate_multiply_assign(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder){return NULL;}
    // TODO: Implement
    llvm::Value* generate_divide_assign(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder){return NULL;}
    // TODO: Implement
    void generate_if(AST *ast, InternalInfo *info, llvm::IRBuilder<> builder){return;}
}