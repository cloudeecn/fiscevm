#ifdef FY_ENGINE_HEADER
#define Cell fy_stack_item


FY_HOT fy_int FY_ENGINE_NAME(
    fy_context *context,
    fy_thread *thread,
    fy_frame *frame,
    fy_int ops,
    fy_exception *exception,
    fy_e2_label_holder **out_labels) {
#ifndef FY_LATE_DECLARATION
#ifdef USE_CFA
  register fy_e2_label cfa;
#endif
  register fy_instruction *ipp;
#ifdef FY_USE_TOS
  register fy_stack_item sppTOS;
#endif
  register fy_stack_item *spp;

  fy_instruction *instructions;
  register fy_stack_item *sbase;


  fy_method *method = NULL;
#endif

#ifndef FY_LATE_DECLARATION
  fy_uint i1, i2, i3, i4, i5, i6, ir1, ir2, ir3, ir4, ir5, ir6;
#endif

  static fy_e2_label_holder labels[] = {
#undef FY_ENGINE_HEADER
#include "fisce-labels.i"
#define FY_ENGINE_HEADER
      {0, -1}
  };
#ifdef MORE_VARS
  MORE_VARS
#endif

#ifdef VM_DEBUG
  FILE *vm_out = stdout;
  const fy_uint vm_debug = 1;
#endif



  if(unlikely(thread == NULL)){
    *out_labels = labels;
    return 0;
  }else{
#ifdef FY_LATE_DECLARATION
#ifdef USE_CFA
  register fy_e2_label cfa;
#endif
  register fy_instruction *ipp;
#ifdef FY_USE_TOS
  register fy_stack_item sppTOS;
#endif
  register fy_stack_item *spp;

  fy_instruction *instructions;
  register fy_stack_item *sbase;

  fy_method *method = NULL;
#endif


  method = frame->method;
  instructions = method->instructions;
  sbase = frame->baseSpp;

#ifdef VM_DEBUG
  if(vm_debug){
    fprintf(vm_out, "\nInvoking: ");
    fy_strFPrint(vm_out, method->uniqueName);
    fprintf(vm_out, " pc = %"FY_PRINT32"d + %"FY_PRINT32"d\n sb = %"FY_PRINT32"d", frame->lpc, frame->pcofs, FY_PDIFF(fy_stack_item, sbase, thread->stack));
    if(frame->lpc + frame->pcofs == FY_IP_begin){
#ifndef FY_LATE_DECLARATION
      fy_uint i1, i2;
      fprintf(vm_out, "[")
      i2 = method->max_locals + method->max_stack;
      for(i1 = 0; i1 < i2; i1++){
        printarg_i(sbase + i1);
        if(i1 < i2 - 1) fputs(", ", vm_out);
      }
      fputs("]\n", vm_out);
#endif
    }
  }
#endif

  if ((method->access_flags & FY_ACC_CLINIT) && frame->lpc + frame->pcofs == FY_IP_begin) {
#ifdef VM_DEBUG
    if(vm_debug){
      fprintf(vm_out, "Checking clinit for: ");
      fy_strFPrint(vm_out, method->uniqueName);
      fprintf(vm_out, "\n");
    }
#endif
    //!CLINIT
    ipp = method->instructions;
    spp = frame->baseSpp + method->max_locals;
    FY_ENGINE_CLINIT(method->owner, 0)
  }

  
  ENGINE_ENTER;
#endif
INST_ADDR(dropout),
INST_ADDR(iload),
INST_ADDR(istore),
INST_ADDR(lload),
INST_ADDR(lstore),
INST_ADDR(iinc),
INST_ADDR(dup),
INST_ADDR(dup_x1),
INST_ADDR(dup_x2),
INST_ADDR(dup2),
INST_ADDR(dup2_x1),
INST_ADDR(dup2_x2),
INST_ADDR(swap),
INST_ADDR(sipush),
INST_ADDR(slpush),
INST_ADDR(d2f),
INST_ADDR(d2i),
INST_ADDR(d2l),
INST_ADDR(f2d),
INST_ADDR(f2i),
INST_ADDR(f2l),
INST_ADDR(i2b),
INST_ADDR(i2c),
INST_ADDR(i2d),
INST_ADDR(i2f),
INST_ADDR(i2l),
INST_ADDR(i2s),
INST_ADDR(l2d),
INST_ADDR(l2f),
INST_ADDR(l2i),
INST_ADDR(fadd),
INST_ADDR(fsub),
INST_ADDR(fmul),
INST_ADDR(fdiv),
INST_ADDR(frem),
INST_ADDR(fneg),
INST_ADDR(fcmpg),
INST_ADDR(fcmpl),
INST_ADDR(iadd),
INST_ADDR(isub),
INST_ADDR(imul),
INST_ADDR(idiv),
INST_ADDR(irem),
INST_ADDR(ineg),
INST_ADDR(iand),
INST_ADDR(ior),
INST_ADDR(ixor),
INST_ADDR(ishl),
INST_ADDR(ishr),
INST_ADDR(iushr),
INST_ADDR(ladd),
INST_ADDR(lsub),
INST_ADDR(lmul),
INST_ADDR(ldiv),
INST_ADDR(lrem),
INST_ADDR(lneg),
INST_ADDR(land),
INST_ADDR(lor),
INST_ADDR(lxor),
INST_ADDR(lcmp),
INST_ADDR(lshl),
INST_ADDR(lshr),
INST_ADDR(lushr),
INST_ADDR(ldc),
INST_ADDR(ldc2_w),
INST_ADDR(arraylength),
INST_ADDR(iaload),
INST_ADDR(iastore),
INST_ADDR(baload),
INST_ADDR(bastore),
INST_ADDR(caload),
INST_ADDR(castore),
INST_ADDR(saload),
INST_ADDR(sastore),
INST_ADDR(laload),
INST_ADDR(lastore),
INST_ADDR(anewarray),
INST_ADDR(multianewarray),
INST_ADDR(new),
INST_ADDR(newarray),
INST_ADDR(getfield),
INST_ADDR(putfield),
INST_ADDR(getfield_x),
INST_ADDR(putfield_x),
INST_ADDR(getstatic),
INST_ADDR(putstatic),
INST_ADDR(getstatic_x),
INST_ADDR(putstatic_x),
INST_ADDR(checkcast),
INST_ADDR(instanceof),
INST_ADDR(monitorenter),
INST_ADDR(monitorexit),
INST_ADDR(invokespecial),
INST_ADDR(invokestatic),
INST_ADDR(invokevirtual),
INST_ADDR(return),
INST_ADDR(ireturn),
INST_ADDR(lreturn),
INST_ADDR(athrow),
INST_ADDR(goto),
INST_ADDR(if_icmpeq),
INST_ADDR(if_icmpne),
INST_ADDR(if_icmplt),
INST_ADDR(if_icmple),
INST_ADDR(if_icmpgt),
INST_ADDR(if_icmpge),
INST_ADDR(ifeq),
INST_ADDR(ifnull),
INST_ADDR(ifne),
INST_ADDR(ifnonnull),
INST_ADDR(iflt),
INST_ADDR(ifle),
INST_ADDR(ifgt),
INST_ADDR(ifge),
INST_ADDR(lookupswitch),
INST_ADDR(tableswitch),
INST_ADDR(dadd),
INST_ADDR(dsub),
INST_ADDR(ddiv),
INST_ADDR(dmul),
INST_ADDR(drem),
INST_ADDR(dneg),
INST_ADDR(dcmpg),
INST_ADDR(dcmpl),
INST_ADDR(nop),
INST_ADDR(pop),
INST_ADDR(pop2),
#ifdef FY_ENGINE_HEADER
    label_fallout_invoke:
    return ops;
  }
}
#undef FY_ENGINE_NAME
#endif
