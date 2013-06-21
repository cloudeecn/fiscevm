function FisceMethod() {
	this.methodId = 0;
	this.accessFlags = 0;
	this.name = "";
	this.descriptor = "";
	this.fullName = "";
	this.uniqueName = "";
	this.owner = undefined;
	this.maxStack = 0;
	this.maxLocals = 0;
	this.codeLength = 0;

	this.code = undefined;
	this.nativeCode = undefined;

	this.exceptionTabls = [];
	this.lineNumberTable = [];

	this.paramCount = 0;
	this.paramTypes = "";
	this.returnType = '';

	this.clinited = false;

	this.refParamTypes = [];
	this.refReturnTypeClass = undefined;
}

function FisceField() {
	this.fieldId = 0;
	this.accessFlags = 0;

	this.constantValueIndex = "";

	this.name = "";
	this.descriptor = "";
	this.fullName = "";
	this.uniqueName = "";

	this.owner = undefined;
	this.type = undefined;

	this.posRel = 0;
	this.posAbs = 0;

}

function FisceClass(type) {
	this.type = type;
	this.phase=0;
	this.needFinalize=false;
	this.constantTypes=[];
	this.constantPool=[];
	
	this.accessFlags=0;

	this.constantClass=undefined;
	this.className="";
	this.constantSuperClass=undefined;
	this.superClass=undefined;
	
	this.constantInterfaces=[];
	this.interfaces=[];
	
	this.fields=[];
	/*BEGIN GC Only*/
	this.staticFields=[];
	this.absFields=[];
	/*END GC Only*/
	
	this.methods=[];
	this.clinit=undefined;
	
	this.sourceFile="";
	
	this.classId=0;
	
	this.sizeRel=0;
	this.sizeAbs=0;
	this.ofsInHeap=0;
	
	this.staticSize=0;
	this.staticArea=undefined;

	this.arr={
			arrayType : 0,
			contentClass : undefined
	};
	
	this.prm={
			pType:''
	};
	
	this.clinitThreadId=0;
	
	this.virtualTable={};
}
FisceClass.TYPE_OBJECT = 0;
FisceClass.TYPE_PRIMITIVE = 1;
FisceClass.TYPE_ARRAY = 2;
FisceClass.ARRAY_TYPE_BYTE=0;
FisceClass.ARRAY_TYPE_SHORT=1;
FisceClass.ARRAY_TYPE_INT=2;
FisceClass.ARRAY_TYPE_LONG=3;
