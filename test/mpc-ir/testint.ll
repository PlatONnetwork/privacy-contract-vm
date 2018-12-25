; ModuleID = 'testint.cpp'
source_filename = "testint.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%"class.platon::mpc::Integer" = type { i32, %"class.platon::mpc::Bit"* }
%"class.platon::mpc::Bit" = type { <2 x i64> }
%"class.google::protobuf::io::CodedInputStream" = type { i8*, i8*, %"class.google::protobuf::io::ZeroCopyInputStream"*, i32, i32, i32, i8, i8, i32, i32, i32, i32, i32, %"class.google::protobuf::DescriptorPool"*, %"class.google::protobuf::MessageFactory"* }
%"class.google::protobuf::io::ZeroCopyInputStream" = type { i32 (...)** }
%"class.google::protobuf::DescriptorPool" = type opaque
%"class.google::protobuf::MessageFactory" = type opaque
%"class.google::protobuf::io::ArrayOutputStream" = type { %"class.google::protobuf::io::ZeroCopyOutputStream", i8*, i32, i32, i32, i32 }
%"class.google::protobuf::io::ZeroCopyOutputStream" = type { i32 (...)** }
%"class.google::protobuf::io::CodedOutputStream" = type <{ %"class.google::protobuf::io::ZeroCopyOutputStream"*, i8*, i32, i32, i8, i8, i8, [5 x i8] }>

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external hidden global i8
@.str = private unnamed_addr constant [20 x i8] c"alice: %d, bob: %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [33 x i8] c"to reveal the value, ret: %d...\0A\00", align 1
@.str.6 = private unnamed_addr constant [24 x i8] c"the ret encode size: %u\00", align 1
@.str.7 = private unnamed_addr constant [33 x i8] c"ListMsg_Main call done, ret: %d\0A\00", align 1
@_ZN6google8protobuf2io16CodedInputStream24default_recursion_limit_E = external local_unnamed_addr global i32, align 4
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_testint.cpp, i8* null }]
@str = private unnamed_addr constant [32 x i8] c"come into, ListMsg_Main...#####\00"
@str.9 = private unnamed_addr constant [21 x i8] c"read the arg0 failed\00"
@str.10 = private unnamed_addr constant [24 x i8] c"ListMsg is will call...\00"
@str.11 = private unnamed_addr constant [20 x i8] c"ListMsg call done .\00"

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*) unnamed_addr #0

; Function Attrs: nounwind
declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*) unnamed_addr #1

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) local_unnamed_addr #2

; Function Attrs: noinline nounwind uwtable
define i32 @_Z7ListMsgii(i32, i32) local_unnamed_addr #3 {
  %3 = alloca %"class.platon::mpc::Integer", align 8
  %4 = alloca %"class.platon::mpc::Integer", align 8
  %5 = alloca %"class.platon::mpc::Integer", align 8
  %6 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i64 0, i64 0), i32 %0, i32 %1)
  %7 = bitcast %"class.platon::mpc::Integer"* %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 16, i8* nonnull %7) #2
  call void @_ZN6platon3mpc7IntegerC1Eiih(%"class.platon::mpc::Integer"* nonnull %3, i32 32, i32 %0, i8 zeroext 1) #2
  %8 = bitcast %"class.platon::mpc::Integer"* %4 to i8*
  call void @llvm.lifetime.start.p0i8(i64 16, i8* nonnull %8) #2
  call void @_ZN6platon3mpc7IntegerC1Eiih(%"class.platon::mpc::Integer"* nonnull %4, i32 32, i32 %1, i8 zeroext 2) #2
  %9 = bitcast %"class.platon::mpc::Integer"* %5 to i8*
  call void @llvm.lifetime.start.p0i8(i64 16, i8* nonnull %9) #2
  call void @_ZNK6platon3mpc7IntegermiERKS1_(%"class.platon::mpc::Integer"* nonnull sret %5, %"class.platon::mpc::Integer"* nonnull %3, %"class.platon::mpc::Integer"* nonnull dereferenceable(16) %4) #2
  %10 = call i32 @_ZNK6platon3mpc7Integer12reveal_int32Ei(%"class.platon::mpc::Integer"* nonnull %5, i32 0) #2
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.1, i64 0, i64 0), i32 %10)
  call void @_ZN6platon3mpc7IntegerD1Ev(%"class.platon::mpc::Integer"* nonnull %5) #2
  call void @llvm.lifetime.end.p0i8(i64 16, i8* nonnull %9) #2
  call void @_ZN6platon3mpc7IntegerD1Ev(%"class.platon::mpc::Integer"* nonnull %4) #2
  call void @llvm.lifetime.end.p0i8(i64 16, i8* nonnull %8) #2
  call void @_ZN6platon3mpc7IntegerD1Ev(%"class.platon::mpc::Integer"* nonnull %3) #2
  call void @llvm.lifetime.end.p0i8(i64 16, i8* nonnull %7) #2
  ret i32 %10
}

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #4

declare void @_ZN6platon3mpc7IntegerC1Eiih(%"class.platon::mpc::Integer"*, i32, i32, i8 zeroext) unnamed_addr #0

declare void @_ZNK6platon3mpc7IntegermiERKS1_(%"class.platon::mpc::Integer"* sret, %"class.platon::mpc::Integer"*, %"class.platon::mpc::Integer"* dereferenceable(16)) local_unnamed_addr #0

declare i32 @_ZNK6platon3mpc7Integer12reveal_int32Ei(%"class.platon::mpc::Integer"*, i32) local_unnamed_addr #0

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #4

; Function Attrs: nounwind
declare void @_ZN6platon3mpc7IntegerD1Ev(%"class.platon::mpc::Integer"*) unnamed_addr #1

; Function Attrs: noinline nounwind uwtable
define i32 @PlatON_entry_ListMsg(i8** nocapture, i8** nocapture readonly, i32* nocapture readonly) local_unnamed_addr #3 {
  %4 = alloca %"class.google::protobuf::io::CodedInputStream", align 8
  %5 = alloca %"class.google::protobuf::io::CodedInputStream", align 8
  %6 = alloca [16 x i8], align 16
  %7 = alloca %"class.google::protobuf::io::ArrayOutputStream", align 8
  %8 = alloca %"class.google::protobuf::io::CodedOutputStream", align 8
  %9 = tail call i32 @puts(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @str, i64 0, i64 0))
  %10 = bitcast %"class.google::protobuf::io::CodedInputStream"* %4 to i8*
  call void @llvm.lifetime.start.p0i8(i64 80, i8* nonnull %10) #2
  %11 = load i8*, i8** %1, align 8, !tbaa !2
  %12 = load i32, i32* %2, align 4, !tbaa !6
  %13 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %4, i64 0, i32 0
  store i8* %11, i8** %13, align 8, !tbaa !8
  %14 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %4, i64 0, i32 1
  %15 = sext i32 %12 to i64
  %16 = getelementptr inbounds i8, i8* %11, i64 %15
  store i8* %16, i8** %14, align 8, !tbaa !11
  %17 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %4, i64 0, i32 2
  store %"class.google::protobuf::io::ZeroCopyInputStream"* null, %"class.google::protobuf::io::ZeroCopyInputStream"** %17, align 8, !tbaa !12
  %18 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %4, i64 0, i32 3
  store i32 %12, i32* %18, align 8, !tbaa !13
  %19 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %4, i64 0, i32 4
  %20 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %4, i64 0, i32 8
  %21 = bitcast i32* %19 to i8*
  call void @llvm.memset.p0i8.i64(i8* nonnull %21, i8 0, i64 10, i32 4, i1 false) #2
  store i32 %12, i32* %20, align 8, !tbaa !14
  %22 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %4, i64 0, i32 9
  store i32 0, i32* %22, align 4, !tbaa !15
  %23 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %4, i64 0, i32 10
  store i32 2147483647, i32* %23, align 8, !tbaa !16
  %24 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %4, i64 0, i32 11
  %25 = load i32, i32* @_ZN6google8protobuf2io16CodedInputStream24default_recursion_limit_E, align 4, !tbaa !6
  store i32 %25, i32* %24, align 4, !tbaa !17
  %26 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %4, i64 0, i32 12
  store i32 %25, i32* %26, align 8, !tbaa !18
  %27 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %4, i64 0, i32 13
  %28 = bitcast %"class.google::protobuf::DescriptorPool"** %27 to i8*
  call void @llvm.memset.p0i8.i64(i8* nonnull %28, i8 0, i64 16, i32 8, i1 false) #2
  %29 = icmp sgt i32 %12, 0
  br i1 %29, label %30, label %36, !prof !19

; <label>:30:                                     ; preds = %3
  %31 = load i8, i8* %11, align 1, !tbaa !20
  %32 = icmp sgt i8 %31, -1
  br i1 %32, label %33, label %36

; <label>:33:                                     ; preds = %30
  %34 = zext i8 %31 to i32
  %35 = getelementptr inbounds i8, i8* %11, i64 1
  store i8* %35, i8** %13, align 8, !tbaa !8
  br label %43

; <label>:36:                                     ; preds = %3, %30
  %37 = call i32 @_ZN6google8protobuf2io16CodedInputStream27ReadVarintSizeAsIntFallbackEv(%"class.google::protobuf::io::CodedInputStream"* nonnull %4) #2
  %38 = icmp sgt i32 %37, -1
  br i1 %38, label %39, label %41

; <label>:39:                                     ; preds = %36
  %40 = load i32, i32* @_ZN6google8protobuf2io16CodedInputStream24default_recursion_limit_E, align 4, !tbaa !6
  br label %43

; <label>:41:                                     ; preds = %36
  %42 = call i32 @puts(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.9, i64 0, i64 0))
  br label %141

; <label>:43:                                     ; preds = %39, %33
  %44 = phi i32 [ %25, %33 ], [ %40, %39 ]
  %45 = phi i32 [ %34, %33 ], [ %37, %39 ]
  %46 = bitcast %"class.google::protobuf::io::CodedInputStream"* %5 to i8*
  call void @llvm.lifetime.start.p0i8(i64 80, i8* nonnull %46) #2
  %47 = getelementptr inbounds i8*, i8** %1, i64 1
  %48 = load i8*, i8** %47, align 8, !tbaa !2
  %49 = getelementptr inbounds i32, i32* %2, i64 1
  %50 = load i32, i32* %49, align 4, !tbaa !6
  %51 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %5, i64 0, i32 0
  store i8* %48, i8** %51, align 8, !tbaa !8
  %52 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %5, i64 0, i32 1
  %53 = sext i32 %50 to i64
  %54 = getelementptr inbounds i8, i8* %48, i64 %53
  store i8* %54, i8** %52, align 8, !tbaa !11
  %55 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %5, i64 0, i32 2
  store %"class.google::protobuf::io::ZeroCopyInputStream"* null, %"class.google::protobuf::io::ZeroCopyInputStream"** %55, align 8, !tbaa !12
  %56 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %5, i64 0, i32 3
  store i32 %50, i32* %56, align 8, !tbaa !13
  %57 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %5, i64 0, i32 4
  %58 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %5, i64 0, i32 8
  %59 = bitcast i32* %57 to i8*
  call void @llvm.memset.p0i8.i64(i8* nonnull %59, i8 0, i64 10, i32 4, i1 false) #2
  store i32 %50, i32* %58, align 8, !tbaa !14
  %60 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %5, i64 0, i32 9
  store i32 0, i32* %60, align 4, !tbaa !15
  %61 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %5, i64 0, i32 10
  store i32 2147483647, i32* %61, align 8, !tbaa !16
  %62 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %5, i64 0, i32 11
  store i32 %44, i32* %62, align 4, !tbaa !17
  %63 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %5, i64 0, i32 12
  store i32 %44, i32* %63, align 8, !tbaa !18
  %64 = getelementptr inbounds %"class.google::protobuf::io::CodedInputStream", %"class.google::protobuf::io::CodedInputStream"* %5, i64 0, i32 13
  %65 = bitcast %"class.google::protobuf::DescriptorPool"** %64 to i8*
  call void @llvm.memset.p0i8.i64(i8* nonnull %65, i8 0, i64 16, i32 8, i1 false) #2
  %66 = icmp sgt i32 %50, 0
  br i1 %66, label %67, label %73, !prof !19

; <label>:67:                                     ; preds = %43
  %68 = load i8, i8* %48, align 1, !tbaa !20
  %69 = icmp sgt i8 %68, -1
  br i1 %69, label %70, label %73

; <label>:70:                                     ; preds = %67
  %71 = zext i8 %68 to i32
  %72 = getelementptr inbounds i8, i8* %48, i64 1
  store i8* %72, i8** %51, align 8, !tbaa !8
  br label %78

; <label>:73:                                     ; preds = %43, %67
  %74 = call i32 @_ZN6google8protobuf2io16CodedInputStream27ReadVarintSizeAsIntFallbackEv(%"class.google::protobuf::io::CodedInputStream"* nonnull %5) #2
  %75 = icmp sgt i32 %74, -1
  br i1 %75, label %78, label %76

; <label>:76:                                     ; preds = %73
  %77 = call i32 @puts(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.9, i64 0, i64 0))
  br label %139

; <label>:78:                                     ; preds = %70, %73
  %79 = phi i32 [ %71, %70 ], [ %74, %73 ]
  %80 = call i32 @puts(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @str.10, i64 0, i64 0))
  %81 = call i32 @_Z7ListMsgii(i32 %45, i32 %79)
  %82 = call i32 @puts(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.11, i64 0, i64 0))
  %83 = getelementptr inbounds [16 x i8], [16 x i8]* %6, i64 0, i64 0
  call void @llvm.lifetime.start.p0i8(i64 16, i8* nonnull %83) #2
  call void @llvm.memset.p0i8.i64(i8* nonnull %83, i8 0, i64 16, i32 16, i1 false)
  %84 = bitcast %"class.google::protobuf::io::ArrayOutputStream"* %7 to i8*
  call void @llvm.lifetime.start.p0i8(i64 32, i8* nonnull %84) #2
  call void @_ZN6google8protobuf2io17ArrayOutputStreamC1EPvii(%"class.google::protobuf::io::ArrayOutputStream"* nonnull %7, i8* nonnull %83, i32 16, i32 -1) #2
  %85 = bitcast %"class.google::protobuf::io::CodedOutputStream"* %8 to i8*
  call void @llvm.lifetime.start.p0i8(i64 32, i8* nonnull %85) #2
  %86 = getelementptr inbounds %"class.google::protobuf::io::ArrayOutputStream", %"class.google::protobuf::io::ArrayOutputStream"* %7, i64 0, i32 0
  call void @_ZN6google8protobuf2io17CodedOutputStreamC1EPNS1_20ZeroCopyOutputStreamE(%"class.google::protobuf::io::CodedOutputStream"* nonnull %8, %"class.google::protobuf::io::ZeroCopyOutputStream"* nonnull %86) #2
  %87 = sext i32 %81 to i64
  %88 = getelementptr inbounds %"class.google::protobuf::io::CodedOutputStream", %"class.google::protobuf::io::CodedOutputStream"* %8, i64 0, i32 2
  %89 = load i32, i32* %88, align 8, !tbaa !21
  %90 = icmp sgt i32 %89, 9
  br i1 %90, label %91, label %122

; <label>:91:                                     ; preds = %78
  %92 = getelementptr inbounds %"class.google::protobuf::io::CodedOutputStream", %"class.google::protobuf::io::CodedOutputStream"* %8, i64 0, i32 1
  %93 = load i8*, i8** %92, align 8, !tbaa !23
  %94 = icmp ugt i32 %81, 127
  %95 = trunc i32 %81 to i8
  br i1 %94, label %96, label %108

; <label>:96:                                     ; preds = %91
  br label %97

; <label>:97:                                     ; preds = %96, %97
  %98 = phi i8 [ %105, %97 ], [ %95, %96 ]
  %99 = phi i8* [ %103, %97 ], [ %93, %96 ]
  %100 = phi i64 [ %102, %97 ], [ %87, %96 ]
  %101 = or i8 %98, -128
  store i8 %101, i8* %99, align 1, !tbaa !20
  %102 = lshr i64 %100, 7
  %103 = getelementptr inbounds i8, i8* %99, i64 1
  %104 = icmp ugt i64 %100, 16383
  %105 = trunc i64 %102 to i8
  br i1 %104, label %97, label %106

; <label>:106:                                    ; preds = %97
  %107 = trunc i64 %102 to i8
  br label %108

; <label>:108:                                    ; preds = %106, %91
  %109 = phi i8* [ %93, %91 ], [ %103, %106 ]
  %110 = phi i8 [ %95, %91 ], [ %107, %106 ]
  store i8 %110, i8* %109, align 1, !tbaa !20
  %111 = getelementptr inbounds i8, i8* %109, i64 1
  %112 = ptrtoint i8* %111 to i64
  %113 = ptrtoint i8* %93 to i64
  %114 = sub i64 %112, %113
  %115 = trunc i64 %114 to i32
  %116 = load i8*, i8** %92, align 8, !tbaa !23
  %117 = shl i64 %114, 32
  %118 = ashr exact i64 %117, 32
  %119 = getelementptr inbounds i8, i8* %116, i64 %118
  store i8* %119, i8** %92, align 8, !tbaa !23
  %120 = load i32, i32* %88, align 8, !tbaa !21
  %121 = sub nsw i32 %120, %115
  store i32 %121, i32* %88, align 8, !tbaa !21
  br label %124

; <label>:122:                                    ; preds = %78
  call void @_ZN6google8protobuf2io17CodedOutputStream21WriteVarint64SlowPathEm(%"class.google::protobuf::io::CodedOutputStream"* nonnull %8, i64 %87) #2
  %123 = load i32, i32* %88, align 8, !tbaa !21
  br label %124

; <label>:124:                                    ; preds = %108, %122
  %125 = phi i32 [ %121, %108 ], [ %123, %122 ]
  %126 = getelementptr inbounds %"class.google::protobuf::io::CodedOutputStream", %"class.google::protobuf::io::CodedOutputStream"* %8, i64 0, i32 3
  %127 = load i32, i32* %126, align 4, !tbaa !24
  %128 = sub nsw i32 %127, %125
  %129 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.6, i64 0, i64 0), i32 %128)
  %130 = load i32, i32* %126, align 4, !tbaa !24
  %131 = load i32, i32* %88, align 8, !tbaa !21
  %132 = sub nsw i32 %130, %131
  %133 = sext i32 %132 to i64
  %134 = call noalias i8* @malloc(i64 %133) #2
  store i8* %134, i8** %0, align 8, !tbaa !2
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %134, i8* nonnull %83, i64 %133, i32 1, i1 false)
  %135 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str.7, i64 0, i64 0), i32 %81)
  %136 = load i32, i32* %126, align 4, !tbaa !24
  %137 = load i32, i32* %88, align 8, !tbaa !21
  %138 = sub nsw i32 %136, %137
  call void @_ZN6google8protobuf2io17CodedOutputStreamD1Ev(%"class.google::protobuf::io::CodedOutputStream"* nonnull %8) #2
  call void @llvm.lifetime.end.p0i8(i64 32, i8* nonnull %85) #2
  call void @llvm.lifetime.end.p0i8(i64 32, i8* nonnull %84) #2
  call void @llvm.lifetime.end.p0i8(i64 16, i8* nonnull %83) #2
  br label %139

; <label>:139:                                    ; preds = %124, %76
  %140 = phi i32 [ %138, %124 ], [ 0, %76 ]
  call void @_ZN6google8protobuf2io16CodedInputStreamD1Ev(%"class.google::protobuf::io::CodedInputStream"* nonnull %5) #2
  call void @llvm.lifetime.end.p0i8(i64 80, i8* nonnull %46) #2
  br label %141

; <label>:141:                                    ; preds = %139, %41
  %142 = phi i32 [ %140, %139 ], [ 0, %41 ]
  call void @_ZN6google8protobuf2io16CodedInputStreamD1Ev(%"class.google::protobuf::io::CodedInputStream"* nonnull %4) #2
  call void @llvm.lifetime.end.p0i8(i64 80, i8* nonnull %10) #2
  ret i32 %142
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #4

declare void @_ZN6google8protobuf2io17ArrayOutputStreamC1EPvii(%"class.google::protobuf::io::ArrayOutputStream"*, i8*, i32, i32) unnamed_addr #0

declare void @_ZN6google8protobuf2io17CodedOutputStreamC1EPNS1_20ZeroCopyOutputStreamE(%"class.google::protobuf::io::CodedOutputStream"*, %"class.google::protobuf::io::ZeroCopyOutputStream"*) unnamed_addr #0

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) local_unnamed_addr #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #4

; Function Attrs: nounwind
declare void @_ZN6google8protobuf2io17CodedOutputStreamD1Ev(%"class.google::protobuf::io::CodedOutputStream"*) unnamed_addr #1

; Function Attrs: nounwind
declare void @_ZN6google8protobuf2io16CodedInputStreamD1Ev(%"class.google::protobuf::io::CodedInputStream"*) unnamed_addr #1

declare i32 @_ZN6google8protobuf2io16CodedInputStream27ReadVarintSizeAsIntFallbackEv(%"class.google::protobuf::io::CodedInputStream"*) local_unnamed_addr #0

declare void @_ZN6google8protobuf2io17CodedOutputStream21WriteVarint64SlowPathEm(%"class.google::protobuf::io::CodedOutputStream"*, i64) local_unnamed_addr #0

; Function Attrs: nounwind uwtable
define internal void @_GLOBAL__sub_I_testint.cpp() #5 section ".text.startup" {
  tail call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* nonnull @_ZStL8__ioinit) #2
  %1 = tail call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init", %"class.std::ios_base::Init"* @_ZStL8__ioinit, i64 0, i32 0), i8* nonnull @__dso_handle) #2
  ret void
}

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) local_unnamed_addr #2

attributes #0 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }
attributes #3 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { argmemonly nounwind }
attributes #5 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (tags/RELEASE_601/final)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"any pointer", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C++ TBAA"}
!6 = !{!7, !7, i64 0}
!7 = !{!"int", !4, i64 0}
!8 = !{!9, !3, i64 0}
!9 = !{!"_ZTSN6google8protobuf2io16CodedInputStreamE", !3, i64 0, !3, i64 8, !3, i64 16, !7, i64 24, !7, i64 28, !7, i64 32, !10, i64 36, !10, i64 37, !7, i64 40, !7, i64 44, !7, i64 48, !7, i64 52, !7, i64 56, !3, i64 64, !3, i64 72}
!10 = !{!"bool", !4, i64 0}
!11 = !{!9, !3, i64 8}
!12 = !{!9, !3, i64 16}
!13 = !{!9, !7, i64 24}
!14 = !{!9, !7, i64 40}
!15 = !{!9, !7, i64 44}
!16 = !{!9, !7, i64 48}
!17 = !{!9, !7, i64 52}
!18 = !{!9, !7, i64 56}
!19 = !{!"branch_weights", i32 2000, i32 1}
!20 = !{!4, !4, i64 0}
!21 = !{!22, !7, i64 16}
!22 = !{!"_ZTSN6google8protobuf2io17CodedOutputStreamE", !3, i64 0, !3, i64 8, !7, i64 16, !7, i64 20, !10, i64 24, !10, i64 25, !10, i64 26}
!23 = !{!22, !3, i64 8}
!24 = !{!22, !7, i64 20}
