// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// http://code.google.com/p/protobuf/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#include <map>
#include <string>

#include <google/protobuf/compiler/javamicro/javamicro_primitive_field.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/compiler/javamicro/javamicro_helpers.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/stubs/strutil.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace javamicro {

using internal::WireFormat;
using internal::WireFormatLite;

namespace {

const char* PrimitiveTypeName(JavaType type) {
  switch (type) {
    case JAVATYPE_INT    : return "int";
    case JAVATYPE_LONG   : return "long";
    case JAVATYPE_FLOAT  : return "float";
    case JAVATYPE_DOUBLE : return "double";
    case JAVATYPE_BOOLEAN: return "boolean";
    case JAVATYPE_STRING : return "java.lang.String";
    case JAVATYPE_BYTES  : return "com.google.protobuf.micro.ByteStringMicro";
    case JAVATYPE_ENUM   : return NULL;
    case JAVATYPE_MESSAGE: return NULL;

    // No default because we want the compiler to complain if any new
    // JavaTypes are added.
  }

  GOOGLE_LOG(FATAL) << "Can't get here.";
  return NULL;
}

bool IsReferenceType(JavaType type) {
  switch (type) {
    case JAVATYPE_INT    : return false;
    case JAVATYPE_LONG   : return false;
    case JAVATYPE_FLOAT  : return false;
    case JAVATYPE_DOUBLE : return false;
    case JAVATYPE_BOOLEAN: return false;
    case JAVATYPE_STRING : return true;
    case JAVATYPE_BYTES  : return true;
    case JAVATYPE_ENUM   : return false;
    case JAVATYPE_MESSAGE: return true;

    // No default because we want the compiler to complain if any new
    // JavaTypes are added.
  }

  GOOGLE_LOG(FATAL) << "Can't get here.";
  return false;
}

const char* GetCapitalizedType(const FieldDescriptor* field) {
  switch (field->type()) {
    case FieldDescriptor::TYPE_INT32   : return "Int32"   ;
    case FieldDescriptor::TYPE_UINT32  : return "UInt32"  ;
    case FieldDescriptor::TYPE_SINT32  : return "SInt32"  ;
    case FieldDescriptor::TYPE_FIXED32 : return "Fixed32" ;
    case FieldDescriptor::TYPE_SFIXED32: return "SFixed32";
    case FieldDescriptor::TYPE_INT64   : return "Int64"   ;
    case FieldDescriptor::TYPE_UINT64  : return "UInt64"  ;
    case FieldDescriptor::TYPE_SINT64  : return "SInt64"  ;
    case FieldDescriptor::TYPE_FIXED64 : return "Fixed64" ;
    case FieldDescriptor::TYPE_SFIXED64: return "SFixed64";
    case FieldDescriptor::TYPE_FLOAT   : return "Float"   ;
    case FieldDescriptor::TYPE_DOUBLE  : return "Double"  ;
    case FieldDescriptor::TYPE_BOOL    : return "Bool"    ;
    case FieldDescriptor::TYPE_STRING  : return "String"  ;
    case FieldDescriptor::TYPE_BYTES   : return "Bytes"   ;
    case FieldDescriptor::TYPE_ENUM    : return "Enum"    ;
    case FieldDescriptor::TYPE_GROUP   : return "Group"   ;
    case FieldDescriptor::TYPE_MESSAGE : return "Message" ;

    // No default because we want the compiler to complain if any new
    // types are added.
  }

  GOOGLE_LOG(FATAL) << "Can't get here.";
  return NULL;
}

// For encodings with fixed sizes, returns that size in bytes.  Otherwise
// returns -1.
int FixedSize(FieldDescriptor::Type type) {
  switch (type) {
    case FieldDescriptor::TYPE_INT32   : return -1;
    case FieldDescriptor::TYPE_INT64   : return -1;
    case FieldDescriptor::TYPE_UINT32  : return -1;
    case FieldDescriptor::TYPE_UINT64  : return -1;
    case FieldDescriptor::TYPE_SINT32  : return -1;
    case FieldDescriptor::TYPE_SINT64  : return -1;
    case FieldDescriptor::TYPE_FIXED32 : return WireFormatLite::kFixed32Size;
    case FieldDescriptor::TYPE_FIXED64 : return WireFormatLite::kFixed64Size;
    case FieldDescriptor::TYPE_SFIXED32: return WireFormatLite::kSFixed32Size;
    case FieldDescriptor::TYPE_SFIXED64: return WireFormatLite::kSFixed64Size;
    case FieldDescriptor::TYPE_FLOAT   : return WireFormatLite::kFloatSize;
    case FieldDescriptor::TYPE_DOUBLE  : return WireFormatLite::kDoubleSize;

    case FieldDescriptor::TYPE_BOOL    : return WireFormatLite::kBoolSize;
    case FieldDescriptor::TYPE_ENUM    : return -1;

    case FieldDescriptor::TYPE_STRING  : return -1;
    case FieldDescriptor::TYPE_BYTES   : return -1;
    case FieldDescriptor::TYPE_GROUP   : return -1;
    case FieldDescriptor::TYPE_MESSAGE : return -1;

    // No default because we want the compiler to complain if any new
    // types are added.
  }
  GOOGLE_LOG(FATAL) << "Can't get here.";
  return -1;
}

// Return true if the type is a that has variable length
// for instance String's.
bool IsVariableLenType(JavaType type) {
  switch (type) {
    case JAVATYPE_INT    : return false;
    case JAVATYPE_LONG   : return false;
    case JAVATYPE_FLOAT  : return false;
    case JAVATYPE_DOUBLE : return false;
    case JAVATYPE_BOOLEAN: return false;
    case JAVATYPE_STRING : return true;
    case JAVATYPE_BYTES  : return true;
    case JAVATYPE_ENUM   : return false;
    case JAVATYPE_MESSAGE: return true;

    // No default because we want the compiler to complain if any new
    // JavaTypes are added.
  }

  GOOGLE_LOG(FATAL) << "Can't get here.";
  return false;
}

bool IsFastStringHandling(const FieldDescriptor* descriptor,
      const Params params) {
  return ((params.optimization() == JAVAMICRO_OPT_SPEED)
      && (GetJavaType(descriptor) == JAVATYPE_STRING));
}

void SetPrimitiveVariables(const FieldDescriptor* descriptor, const Params params,
                           map<string, string>* variables) {
  (*variables)["name"] =
    UnderscoresToCamelCase(descriptor);
  (*variables)["capitalized_name"] =
    UnderscoresToCapitalizedCamelCase(descriptor);
  (*variables)["number"] = SimpleItoa(descriptor->number());
  (*variables)["type"] = PrimitiveTypeName(GetJavaType(descriptor));
  (*variables)["default"] = DefaultValue(params, descriptor);
  (*variables)["boxed_type"] = BoxedPrimitiveTypeName(GetJavaType(descriptor));
  (*variables)["capitalized_type"] = GetCapitalizedType(descriptor);
  (*variables)["tag"] = SimpleItoa(WireFormat::MakeTag(descriptor));
  (*variables)["tag_size"] = SimpleItoa(
      WireFormat::TagSize(descriptor->number(), descriptor->type()));
  if (IsReferenceType(GetJavaType(descriptor))) {
    (*variables)["null_check"] =
        "  if (value == null) {\n"
        "      return this;\n"
		    "    }\n  ";
  } else {
    (*variables)["null_check"] = "";
  }
  int fixed_size = FixedSize(descriptor->type());
  if (fixed_size != -1) {
    (*variables)["fixed_size"] = SimpleItoa(fixed_size);
  }
  (*variables)["message_name"] = descriptor->containing_type()->name();
  (*variables)["original_name"] = descriptor->name();
  /*if (descriptor->type() == FieldDescriptor::TYPE_BYTES) {
    (*variables)["bytes_method"] = ".toStringUtf8()";
  } else {
    (*variables)["bytes_method"] = "";
  }*/
}
}  // namespace

// ===================================================================

PrimitiveFieldGenerator::
PrimitiveFieldGenerator(const FieldDescriptor* descriptor, const Params& params)
  : FieldGenerator(params), descriptor_(descriptor) {
  SetPrimitiveVariables(descriptor, params, &variables_);
}

PrimitiveFieldGenerator::~PrimitiveFieldGenerator() {}

void PrimitiveFieldGenerator::
GenerateToUriCode(io::Printer* printer) const {
  bool has_default_value = descriptor_->has_default_value();
  if (!has_default_value) {
    printer->Print(variables_,
      "if (has$capitalized_name$()) {\n");
    printer->Indent();
  }
  printer->Print(variables_,
    "prefixAndChar(query);\n");
  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType) {
    case JAVATYPE_BYTES:
      printer->Print(variables_,
        "query.append(\"$original_name$\").append(\"=\").append(get$capitalized_name$().toStringUtf8());\n");
      break;
    default:
      printer->Print(variables_,
        "query.append(\"$original_name$\").append(\"=\").append(get$capitalized_name$());\n");
      break;
  }

  if (!has_default_value) {
    printer->Outdent();
    printer->Print("}\n");
  }
}

void PrimitiveFieldGenerator::
GenerateToBundleCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (has$capitalized_name$()) {\n");
  printer->Indent();

  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType) {
  case JAVATYPE_INT:
    printer->Print(variables_,
      "bundle.putInt(\"$original_name$\", get$capitalized_name$());\n");
    break;
  case JAVATYPE_LONG:
    printer->Print(variables_,
      "bundle.putLong(\"$original_name$\", get$capitalized_name$());\n");
    break;
  case JAVATYPE_BOOLEAN:
    printer->Print(variables_,
      "bundle.putBoolean(\"$original_name$\", get$capitalized_name$());\n");
    break;
  case JAVATYPE_FLOAT:
    printer->Print(variables_,
      "bundle.putFloat(\"$original_name$\", get$capitalized_name$());\n");
    break;
  case JAVATYPE_DOUBLE:
    printer->Print(variables_,
      "bundle.putDouble(\"$original_name$\", get$capitalized_name$());\n");
    break;
  case JAVATYPE_STRING:
    printer->Print(variables_,
      "String value = get$capitalized_name$();\n"
      "if (value != null) {\n"
      "  bundle.putString(\"$original_name$\", value);\n"
      "}\n");
    break;
  case JAVATYPE_BYTES:
    printer->Print(variables_,
      "com.google.protobuf.micro.ByteStringMicro value = get$capitalized_name$();\n"
      "if (value != null) {\n"
      "  bundle.putByteArray(\"$original_name$\", value.toByteArray());\n"
      "}\n");
    break;
  default:
    GOOGLE_LOG(FATAL) << "Can't get here.";
    break;
  }

  printer->Outdent();
  printer->Print("}\n");
}

void PrimitiveFieldGenerator::
GenerateFromBundleCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (bundle.containsKey(\"$original_name$\")) {\n");
  printer->Indent();

  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType) {
  case JAVATYPE_INT:
    printer->Print(variables_,
      "result.set$capitalized_name$(bundle.getInt(\"$original_name$\"));\n");
    break;
  case JAVATYPE_LONG:
    printer->Print(variables_,
      "result.set$capitalized_name$(bundle.getLong(\"$original_name$\"));\n");
    break;
  case JAVATYPE_BOOLEAN:
    printer->Print(variables_,
      "result.set$capitalized_name$(bundle.getBoolean(\"$original_name$\"));\n");
    break;
  case JAVATYPE_FLOAT:
    printer->Print(variables_,
      "result.set$capitalized_name$(bundle.getFloat(\"$original_name$\"));\n");
    break;
  case JAVATYPE_DOUBLE:
    printer->Print(variables_,
      "result.set$capitalized_name$(bundle.getDouble(\"$original_name$\"));\n");
    break;
  case JAVATYPE_STRING:
    printer->Print(variables_,
      "String value = bundle.getString(\"$original_name$\");\n"
      "if (value != null) {\n"
      "  result.set$capitalized_name$(value);\n"
      "}\n");
    break;
  case JAVATYPE_BYTES:
    printer->Print(variables_,
      "byte[] array = bundle.getByteArray(\"$original_name$\");\n"
      "if (array != null && array.length > 0) {\n"
      "  result.set$capitalized_name$(com.google.protobuf.micro.ByteStringMicro.copyFrom(array));\n"
      "}\n");
    break;
  default:
    GOOGLE_LOG(FATAL) << "Can't get here.";
    break;
  }

  printer->Outdent();
  printer->Print("}\n");
}

void PrimitiveFieldGenerator::
GenerateWriteToParcelCode(io::Printer* printer) const {
  printer->Print("{\n");
  printer->Indent();
  printer->Print(variables_,
    "dest.writeByte((byte) (has$capitalized_name$ ? 1 : 0));\n");

  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType)
  {
  case JAVATYPE_INT:
    printer->Print(variables_,
      "dest.writeInt($name$_);\n");
    break;
  case JAVATYPE_LONG:
    printer->Print(variables_,
      "dest.writeLong($name$_);\n");
    break;
  case JAVATYPE_FLOAT:
    printer->Print(variables_,
      "dest.writeFloat($name$_);\n");
    break;
  case JAVATYPE_DOUBLE:
    printer->Print(variables_,
      "dest.writeDouble($name$_);\n");
    break;
  case JAVATYPE_BOOLEAN:
    printer->Print(variables_,
      "dest.writeByte((byte) ($name$_ ? 1 : 0));\n");
    break;
  case JAVATYPE_STRING:
    printer->Print(variables_,
      "dest.writeString($name$_);\n");
    break;
  case JAVATYPE_BYTES:
    printer->Print(variables_,
      "dest.writeString($name$_.toStringUtf8());\n");
    break;
  default:
    GOOGLE_LOG(FATAL) << "Can't get here.";
    break;
  }

  printer->Outdent();
  printer->Print("}\n");
}

void PrimitiveFieldGenerator::
GenerateParcelableConstructorCode(io::Printer* printer) const {
  printer->Print("{\n");
  printer->Indent();
  printer->Print(variables_,
    "has$capitalized_name$ = source.readByte() == 1;\n");

  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType)
  {
  case JAVATYPE_INT:
    printer->Print(variables_,
      "$name$_ = source.readInt();\n");
    break;
  case JAVATYPE_LONG:
    printer->Print(variables_,
      "$name$_ = source.readLong();\n");
    break;
  case JAVATYPE_FLOAT:
    printer->Print(variables_,
      "$name$_ = source.readFloat();\n");
    break;
  case JAVATYPE_DOUBLE:
    printer->Print(variables_,
      "$name$_ = source.readDouble();\n");
    break;
  case JAVATYPE_BOOLEAN:
    printer->Print(variables_,
      "$name$_ = source.readByte() == 1;\n");
    break;
  case JAVATYPE_STRING:
    printer->Print(variables_,
      "$name$_ = source.readString();\n");
    break;
  case JAVATYPE_BYTES:
    printer->Print(variables_,
      "$name$_ = com.google.protobuf.micro.ByteStringMicro\n"
      "            .copyFromUtf8(source.readString());\n");
    break;
  default:
    GOOGLE_LOG(FATAL) << "Can't get here.";
    break;
  }

  printer->Outdent();
  printer->Print("}\n");
}

void PrimitiveFieldGenerator::
GenerateFromJsonCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (json.has(\"$original_name$\")) {\n");
  printer->Indent();

  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType) {
  case JAVATYPE_INT:
    printer->Print(variables_,
      "result.set$capitalized_name$(json.getInt(\"$original_name$\"));\n");
    break;
  case JAVATYPE_LONG:
    printer->Print(variables_,
      "result.set$capitalized_name$(json.getLong(\"$original_name$\"));\n");
    break;
  case JAVATYPE_FLOAT:
    printer->Print(variables_,
      "result.set$capitalized_name$((float) json.getDouble(\"$original_name$\"));\n");
    break;
  case JAVATYPE_DOUBLE:
    printer->Print(variables_,
      "result.set$capitalized_name$(json.getDouble(\"$original_name$\"));\n");
    break;
  case JAVATYPE_BOOLEAN:
    printer->Print(variables_,
      "result.set$capitalized_name$(json.getBoolean(\"$original_name$\"));\n");
    break;
  case JAVATYPE_STRING:
    printer->Print(variables_,
      "String value = json.getString(\"$original_name$\");\n"
      "if (value != null) {\n"
      "  result.set$capitalized_name$(value);\n"
      "}\n");
    break;
  case JAVATYPE_BYTES:
    printer->Print(variables_,
      "String value = json.getString(\"$original_name$\");\n"
      "if (value != null) {\n"
      "  result.set$capitalized_name$(\n"
      "          com.google.protobuf.micro.ByteStringMicro.copyFromUtf8(value));\n"
      "}\n");
    break;
  default:
    GOOGLE_LOG(FATAL) << "Can't get here.";
    break;
  }

  printer->Outdent();
  printer->Print("}\n");
}

void PrimitiveFieldGenerator::
GenerateToJsonCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (has$capitalized_name$()) {\n");
  printer->Indent();

  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType)
  {
  case JAVATYPE_STRING:
    printer->Print(variables_,
      "String value = get$capitalized_name$();\n"
      "if (value != null) {\n"
      "  stringer.key(\"$original_name$\").value(value);\n"
      "}\n");
    break;
  case JAVATYPE_BYTES:
    printer->Print(variables_,
      "com.google.protobuf.micro.ByteStringMicro value = get$capitalized_name$();\n"
      "if (value != null) {\n"
      "  stringer.key(\"$original_name$\").value(value.toStringUtf8());\n"
      "}\n");
    break;
  default:
    printer->Print(variables_,
      "stringer.key(\"$original_name$\").value(get$capitalized_name$());\n");
    break;
  }
  
  printer->Outdent();
  printer->Print("}\n");
}

void PrimitiveFieldGenerator::
GenerateMembers(io::Printer* printer) const {
  printer->Print(variables_,
    "private boolean has$capitalized_name$;\n"
    "private $type$ $name$_ = $default$;\n"
    "public $type$ get$capitalized_name$() { return $name$_; }\n"
    "public boolean has$capitalized_name$() { return has$capitalized_name$; }\n");
  if (IsFastStringHandling(descriptor_, params_)) {
    printer->Print(variables_,
      "private byte [] $name$Utf8_ = null;\n"
      "public $message_name$ set$capitalized_name$($type$ value) {\n"
      "  has$capitalized_name$ = true;\n"
      "  $name$_ = value;\n"
      "  $name$Utf8_ = null;\n"
      "  return this;\n"
      "}\n"
      "public $message_name$ clear$capitalized_name$() {\n"
      "  has$capitalized_name$ = false;\n"
      "  $name$_ = $default$;\n"
      "  $name$Utf8_ = null;\n"
      "  return this;\n"
      "}\n");
  } else {
    if (IsVariableLenType(GetJavaType(descriptor_))) {
      printer->Print(variables_,
        "public $message_name$ set$capitalized_name$($type$ value) {\n"
        "  if (value == null) {\n"
        "    return clear$capitalized_name$();\n"
        "  }\n"
        "  has$capitalized_name$ = true;\n"
        "  $name$_ = value;\n"
        "  return this;\n"
        "}\n"
        "public $message_name$ clear$capitalized_name$() {\n"
        "  has$capitalized_name$ = false;\n"
        "  $name$_ = $default$;\n"
        "  return this;\n"
        "}\n");
    } else {
      printer->Print(variables_,
        "public $message_name$ set$capitalized_name$($type$ value) {\n"
        "  has$capitalized_name$ = true;\n"
        "  $name$_ = value;\n"
        "  return this;\n"
        "}\n"
        "public $message_name$ clear$capitalized_name$() {\n"
        "  has$capitalized_name$ = false;\n"
        "  $name$_ = $default$;\n"
        "  return this;\n"
        "}\n");
    }
  }
}

void PrimitiveFieldGenerator::
GenerateMergingCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (other.has$capitalized_name$()) {\n"
    "  set$capitalized_name$(other.get$capitalized_name$());\n"
    "}\n");
}

void PrimitiveFieldGenerator::
GenerateParsingCode(io::Printer* printer) const {
  printer->Print(variables_,
    "set$capitalized_name$(input.read$capitalized_type$());\n");
}

void PrimitiveFieldGenerator::
GenerateSerializationCode(io::Printer* printer) const {
  if (IsFastStringHandling(descriptor_, params_)) {
    printer->Print(variables_,
      "if (has$capitalized_name$()) {\n"
      "  output.writeByteArray($number$, $name$Utf8_);\n"
      "}\n");
  } else {
    printer->Print(variables_,
      "if (has$capitalized_name$()) {\n"
      "  output.write$capitalized_type$($number$, get$capitalized_name$());\n"
      "}\n");
  }
}

void PrimitiveFieldGenerator::
GenerateSerializedSizeCode(io::Printer* printer) const {
  if (IsFastStringHandling(descriptor_, params_)) {
    printer->Print(variables_,
      "if (has$capitalized_name$()) {\n"
      "  try {\n"
      "    $name$Utf8_ = $name$_.getBytes(\"UTF-8\");\n"
      "  } catch (java.io.UnsupportedEncodingException e) {\n"
      "    throw new RuntimeException(\"UTF-8 not supported.\");\n"
      "  }\n"
      "  size += com.google.protobuf.micro.CodedOutputStreamMicro\n"
      "    .computeByteArraySize($number$, $name$Utf8_);\n"
      "}\n");
  } else {
    printer->Print(variables_,
      "if (has$capitalized_name$()) {\n"
      "  size += com.google.protobuf.micro.CodedOutputStreamMicro\n"
      "    .compute$capitalized_type$Size($number$, get$capitalized_name$());\n"
      "}\n");
  }
}

string PrimitiveFieldGenerator::GetBoxedType() const {
  return BoxedPrimitiveTypeName(GetJavaType(descriptor_));
}

// ===================================================================

RepeatedPrimitiveFieldGenerator::
RepeatedPrimitiveFieldGenerator(const FieldDescriptor* descriptor, const Params& params)
  : FieldGenerator(params), descriptor_(descriptor) {
  SetPrimitiveVariables(descriptor, params, &variables_);
  if (descriptor_->options().packed()) {
    GOOGLE_LOG(FATAL) << "MicroRuntime does not support packed";
  }
}

RepeatedPrimitiveFieldGenerator::~RepeatedPrimitiveFieldGenerator() {}

void RepeatedPrimitiveFieldGenerator::
GenerateToUriCode(io::Printer* printer) const {
  printer->Print(variables_,
    "count = get$capitalized_name$Count();\n"
    "if (count > 0) {\n"
    "  prefixAndChar(query);\n"
    "  query.append(\"(\");\n"
    "  for (int i = 0; i < count; ++i) {\n"
    "    if (i != 0) {\n"
    "      query.append(\",\");\n"
    "    }\n");
  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType) {
  case JAVATYPE_BYTES:
    printer->Print(variables_,
      "    query.append(get$capitalized_name$(i).toStringUtf8());\n");
    break;
  default:
    printer->Print(variables_,
      "    query.append(get$capitalized_name$(i));\n");
    break;
  }
  printer->Print(variables_,
    "  }\n"
    "  query.append(\")\");\n"
    "}\n");
}

void RepeatedPrimitiveFieldGenerator::
GenerateToBundleCode(io::Printer* printer) const {
  printer->Print(variables_,
    "count = get$capitalized_name$Count();\n"
    "if (count > 0) {\n");
  printer->Indent();

  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType) {
  case JAVATYPE_INT:
    printer->Print(variables_,
      "int[] array = new int[count];\n"
      "Integer value;\n"
      "for (int i = 0; i < count; ++i) {\n"
      "  value = get$capitalized_name$(i);\n"
      "  array[i] = value == null ? 0 : value.intValue();\n"
      "}\n"
      "bundle.putIntArray(\"$original_name$\", array);\n");
    break;
  case JAVATYPE_LONG:
    printer->Print(variables_,
      "long[] array = new long[count];\n"
      "Long value;\n"
      "for (int i = 0; i < count; ++i) {\n"
      "  value = get$capitalized_name$(i);\n"
      "  array[i] = value == null ? 0 : value.longValue();\n"
      "}\n"
      "bundle.putLongArray(\"$original_name$\", array);\n");
    break;
  case JAVATYPE_BOOLEAN:
    printer->Print(variables_,
      "boolean[] array = new boolean[count];\n"
      "Boolean value;\n"
      "for (int i = 0; i < count; ++i) {\n"
      "  value = get$capitalized_name$(i);\n"
      "  array[i] = value == null ? false : value.booleanValue();\n"
      "}\n"
      "bundle.putBooleanArray(\"$original_name$\", array);\n");
    break;
  case JAVATYPE_FLOAT:
    printer->Print(variables_,
      "float[] array = new float[count];\n"
      "Float value;\n"
      "for (int i = 0; i < count; ++i) {\n"
      "  value = get$capitalized_name$(i);\n"
      "  array[i] = value == null ? 0.0f : value.floatValue();\n"
      "}\n"
      "bundle.putFloatArray(\"$original_name$\", array);\n");
    break;
  case JAVATYPE_DOUBLE:
    printer->Print(variables_,
      "double[] array = new double[count];\n"
      "Double value;\n"
      "for (int i = 0; i < count; ++i) {\n"
      "  value = get$capitalized_name$(i);\n"
      "  array[i] = value == null ? 0.0 : value.doubleValue();\n"
      "}\n"
      "bundle.putDoubleArray(\"$original_name$\", array);\n");
    break;
  case JAVATYPE_STRING:
    printer->Print(variables_,
      "String[] array = new String[count];\n"
      "for (int i = 0; i < count; ++i) {\n"
      "  array[i] = get$capitalized_name$(i);\n"
      "}\n"
      "bundle.putStringArray(\"$original_name$\", array);\n");
    break;
  case JAVATYPE_BYTES:
    printer->Print(variables_,
      "byte[][] array = new byte[count][];\n"
      "com.google.protobuf.micro.ByteStringMicro value;\n"
      "for (int i = 0; i < count; ++i) {\n"
      "  value = get$capitalized_name$(i);\n"
      "  if (value != null) {\n"
      "    array[i] = value.toByteArray();\n"
      "  }\n"
      "}\n"
      "bundle.putSerializable(\"$original_name$\", array);\n");
    break;
  default:
    GOOGLE_LOG(FATAL) << "Can't get here.";
    break;
  }

  printer->Outdent();
  printer->Print("}\n");
}

void RepeatedPrimitiveFieldGenerator::
GenerateFromBundleCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (bundle.containsKey(\"$original_name$\")) {\n");
  printer->Indent();

  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType) {
  case JAVATYPE_INT:
    printer->Print(variables_,
      "int[] array = bundle.getIntArray(\"$original_name$\");\n"
      "if (array != null) {\n"
      "  count = array.length;\n"
      "  if (count > 0) {\n"
      "    for (int i = 0; i < count; ++i) {\n"
      "      result.add$capitalized_name$(array[i]);\n"
      "    }\n"
      "  }\n"
      "}\n");
    break;
  case JAVATYPE_LONG:
    printer->Print(variables_,
      "long[] array = bundle.getLongArray(\"$original_name$\");\n"
      "if (array != null) {\n"
      "  count = array.length;\n"
      "  if (count > 0) {\n"
      "    for (int i = 0; i < count; ++i) {\n"
      "      result.add$capitalized_name$(array[i]);\n"
      "    }\n"
      "  }\n"
      "}\n");
    break;
  case JAVATYPE_BOOLEAN:
    printer->Print(variables_,
      "boolean[] array = bundle.getBooleanArray(\"$original_name$\");\n"
      "if (array != null) {\n"
      "  count = array.length;\n"
      "  if (count > 0) {\n"
      "    for (int i = 0; i < count; ++i) {\n"
      "      result.add$capitalized_name$(array[i]);\n"
      "    }\n"
      "  }\n"
      "}\n");
    break;
  case JAVATYPE_FLOAT:
    printer->Print(variables_,
      "float[] array = bundle.getFloatArray(\"$original_name$\");\n"
      "if (array != null) {\n"
      "  count = array.length;\n"
      "  if (count > 0) {\n"
      "    for (int i = 0; i < count; ++i) {\n"
      "      result.add$capitalized_name$(array[i]);\n"
      "    }\n"
      "  }\n"
      "}\n");
    break;
  case JAVATYPE_DOUBLE:
    printer->Print(variables_,
      "double[] array = bundle.getDoubleArray(\"$original_name$\");\n"
      "if (array != null) {\n"
      "  count = array.length;\n"
      "  if (count > 0) {\n"
      "    for (int i = 0; i < count; ++i) {\n"
      "      result.add$capitalized_name$(array[i]);\n"
      "    }\n"
      "  }\n"
      "}\n");
    break;
  case JAVATYPE_STRING:
    printer->Print(variables_,
      "String[] array = bundle.getStringArray(\"$original_name$\");\n"
      "if (array != null) {\n"
      "  count = array.length;\n"
      "  if (count > 0) {\n"
      "    for (int i = 0; i < count; ++i) {\n"
      "      result.add$capitalized_name$(array[i]);\n"
      "    }\n"
      "  }\n"
      "}\n");
    break;
  case JAVATYPE_BYTES:
    printer->Print(variables_,
      "byte[][] array = (byte[][]) bundle.getSerializable(\"$original_name$\");\n"
      "if (array != null) {\n"
      "  count = array.length;\n"
      "  if (count > 0) {\n"
      "    byte[] value;\n"
      "    for (int i = 0; i < count; ++i) {\n"
      "      value = array[i];\n"
      "      if (value != null && value.length > 0) {\n"
      "        result.add$capitalized_name$(com.google.protobuf.micro.ByteStringMicro.copyFrom(value));\n"
      "      }\n"
      "    }\n"
      "  }\n"
      "}\n");
    break;
  default:
    GOOGLE_LOG(FATAL) << "Can't get here.";
    break;
  }

  printer->Outdent();
  printer->Print("}\n");
}

void RepeatedPrimitiveFieldGenerator::
GenerateWriteToParcelCode(io::Printer* printer) const {
  printer->Print(variables_,
    "dest.writeList($name$_);\n");
}

void RepeatedPrimitiveFieldGenerator::
GenerateParcelableConstructorCode(io::Printer* printer) const {
  if (IsReferenceType(GetJavaType(descriptor_))) {
    printer->Print(variables_,
      "$name$_ = (java.util.List<$type$>) source.readArrayList(classLoader);\n");
  } else {
    printer->Print(variables_,
      "$name$_ = (java.util.List<$boxed_type$>) source.readArrayList(classLoader);\n");
  }
}

void RepeatedPrimitiveFieldGenerator::
GenerateFromJsonCode(io::Printer* printer) const {
  printer->Print(variables_,
    "if (json.has(\"$original_name$\")) {\n"
    "  array = json.getJSONArray(\"$original_name$\");\n"
    "  count = array.length();\n");
  printer->Indent();

  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType)
  {
  case JAVATYPE_INT:
    printer->Print(variables_,
      "for (int i = 0; i < count; ++i) {\n"
      "  result.add$capitalized_name$(array.getInt(i));\n");
    break;
  case JAVATYPE_LONG:
    printer->Print(variables_,
      "for (int i = 0; i < count; ++i) {\n"
      "  result.add$capitalized_name$(array.getLong(i));\n");
    break;
  case JAVATYPE_FLOAT:
    printer->Print(variables_,
      "for (int i = 0; i < count; ++i) {\n"
      "  result.add$capitalized_name$((float) array.getDouble(i));\n");
    break;
  case JAVATYPE_DOUBLE:
    printer->Print(variables_,
      "for (int i = 0; i < count; ++i) {\n"
      "  result.add$capitalized_name$(array.getDouble(i));\n");
    break;
  case JAVATYPE_BOOLEAN:
    printer->Print(variables_,
      "for (int i = 0; i < count; ++i) {\n"
      "  result.add$capitalized_name$(array.getBoolean(i));\n");
    break;
  case JAVATYPE_STRING:
    printer->Print(variables_,
      "for (int i = 0; i < count; ++i) {\n"
      "  result.add$capitalized_name$(array.getString(i));\n");
    break;
  case JAVATYPE_BYTES:
    printer->Print(variables_,
      "String value;\n"
      "for (int i = 0; i < count; ++i) {\n"
      "  value = array.getString(i);\n"
      "  if (value != null) {\n"
      "    result.add$capitalized_name$(\n"
      "            com.google.protobuf.micro.ByteStringMicro.copyFromUtf8(value));\n"
      "  }\n");
    break;
  default:
    GOOGLE_LOG(FATAL) << "Can't get here.";
    break;
  }

  printer->Outdent();
  printer->Print(
    "  }\n"
    "}\n");
}

void RepeatedPrimitiveFieldGenerator::
GenerateToJsonCode(io::Printer* printer) const {
  printer->Print(variables_,
    "count = get$capitalized_name$Count();\n"
    "if (count > 0) {\n"
    "  stringer.key(\"$original_name$\").array();\n");
  printer->Indent();

  JavaType javaType = GetJavaType(descriptor_);
  switch (javaType) {
  case JAVATYPE_STRING:
    printer->Print(variables_,
      "String value;\n"
      "for (int i = 0; i < count; ++i) {\n"
      "  value = get$capitalized_name$(i);\n"
      "  if (value != null) {\n"
      "    stringer.value(value);\n"
      "  }\n"
      "}\n");
    break;
  case JAVATYPE_BYTES:
    printer->Print(variables_,
      "com.google.protobuf.micro.ByteStringMicro value;\n"
      "for (int i = 0; i < count; ++i) {\n"
      "  value = get$capitalized_name$(i);\n"
      "  if (value != null) {\n"
      "    stringer.value(value.toStringUtf8());\n"
      "  }\n"
      "}\n");
    break;
  default:
    printer->Print(variables_,
      "for (int i = 0; i < count; ++i) {\n"
      "  stringer.value(get$capitalized_name$(i));\n"
      "}\n");
    break;
  }

  printer->Outdent();
  printer->Print(
    "  stringer.endArray();\n"
    "}\n");
}

void RepeatedPrimitiveFieldGenerator::
GenerateMembers(io::Printer* printer) const {
  if (IsFastStringHandling(descriptor_, params_)) {
    if (params_.java_use_vector()) {
      printer->Print(variables_,
        "private java.util.Vector $name$_ = new java.util.Vector();\n"
        "public java.util.Vector get$capitalized_name$List() {\n"
        "  return $name$_;\n"
        "}\n"
        "private java.util.Vector $name$Utf8_ = new java.util.Vector();\n"
        "public int get$capitalized_name$Count() { return $name$_.size(); }\n"
        "public $type$ get$capitalized_name$(int index) {\n"
        "  return (($type$)$name$_.elementAt(index));\n"
        "}\n"
        "public $message_name$ set$capitalized_name$(int index, $type$ value) {\n"
        "$null_check$"
        "  $name$_.setElementAt(value, index);\n"
        "  $name$Utf8_ = null;\n"
        "  return this;\n"
        "}\n"
        "public $message_name$ add$capitalized_name$($type$ value) {\n"
        "$null_check$"
        "  $name$_.addElement(value);\n"
        "  $name$Utf8_ = null;\n"
        "  return this;\n"
        "}\n"
        "public $message_name$ clear$capitalized_name$() {\n"
        "  $name$_.removeAllElements();\n"
        "  $name$Utf8_ = null;\n"
        "  return this;\n"
        "}\n");
    } else {
      printer->Print(variables_,
        "private java.util.List<$type$> $name$_ =\n"
        "  java.util.Collections.emptyList();\n"
        "public java.util.List<$type$> get$capitalized_name$List() {\n"
        "  return $name$_;\n"   // note:  unmodifiable list
        "}\n"
        "private java.util.List<byte []> $name$Utf8_ = null;\n"
        "public int get$capitalized_name$Count() { return $name$_.size(); }\n"
        "public $type$ get$capitalized_name$(int index) {\n"
        "  return $name$_.get(index);\n"
        "}\n"
        "public $message_name$ set$capitalized_name$(int index, $type$ value) {\n"
        "$null_check$"
        "  $name$_.set(index, value);\n"
        "  $name$Utf8_ = null;\n"
        "  return this;\n"
        "}\n"
        "public $message_name$ add$capitalized_name$($type$ value) {\n"
        "$null_check$"
        "  if ($name$_.isEmpty()) {\n"
        "    $name$_ = new java.util.ArrayList<$type$>();\n"
        "  }\n"
        "  $name$_.add(value);\n"
        "  $name$Utf8_ = null;\n"
        "  return this;\n"
        "}\n"
        "public $message_name$ clear$capitalized_name$() {\n"
        "  $name$_ = java.util.Collections.emptyList();\n"
        "  $name$Utf8_ = null;\n"
        "  return this;\n"
        "}\n");
    }
  } else if (params_.java_use_vector()) {
    if (IsReferenceType(GetJavaType(descriptor_))) {
      printer->Print(variables_,
        "private java.util.Vector $name$_ = new java.util.Vector();\n"
        "public java.util.Vector get$capitalized_name$List() {\n"
        "  return $name$_;\n"
        "}\n"
        "public int get$capitalized_name$Count() { return $name$_.size(); }\n"
        "public $type$ get$capitalized_name$(int index) {\n"
        "  return ($type$) $name$_.elementAt(index);\n"
        "}\n"
        "public $message_name$ set$capitalized_name$(int index, $type$ value) {\n"
        "$null_check$"
        "  $name$_.setElementAt(value, index);\n"
        "  return this;\n"
        "}\n"
        "public $message_name$ add$capitalized_name$($type$ value) {\n"
        "$null_check$"
        "  $name$_.addElement(value);\n"
        "  return this;\n"
        "}\n"
        "public $message_name$ clear$capitalized_name$() {\n"
        "  $name$_.removeAllElements();\n"
        "  return this;\n"
        "}\n");
    } else {
      printer->Print(variables_,
        "private java.util.Vector $name$_ = new java.util.Vector();\n"
        "public java.util.Vector get$capitalized_name$List() {\n"
        "  return $name$_;\n"
        "}\n"
        "public int get$capitalized_name$Count() { return $name$_.size(); }\n"
        "public $type$ get$capitalized_name$(int index) {\n"
        "  return (($boxed_type$)$name$_.elementAt(index)).$type$Value();\n"
        "}\n"
        "public $message_name$ set$capitalized_name$(int index, $type$ value) {\n"
        "$null_check$"
        "  $name$_.setElementAt(new $boxed_type$(value), index);\n"
        "  return this;\n"
        "}\n"
        "public $message_name$ add$capitalized_name$($type$ value) {\n"
        "$null_check$"
        "  $name$_.addElement(new $boxed_type$(value));\n"
        "  return this;\n"
        "}\n"
        "public $message_name$ clear$capitalized_name$() {\n"
        "  $name$_.removeAllElements();\n"
        "  return this;\n"
        "}\n");
    }
  } else {
    printer->Print(variables_,
      "private java.util.List<$boxed_type$> $name$_ =\n"
      "  java.util.Collections.emptyList();\n"
      "public java.util.List<$boxed_type$> get$capitalized_name$List() {\n"
      "  return $name$_;\n"   // note:  unmodifiable list
      "}\n"
      "public int get$capitalized_name$Count() { return $name$_.size(); }\n"
      "public $type$ get$capitalized_name$(int index) {\n"
      "  return $name$_.get(index);\n"
      "}\n"
      "public $message_name$ set$capitalized_name$(int index, $type$ value) {\n"
      "$null_check$"
      "  $name$_.set(index, value);\n"
      "  return this;\n"
      "}\n"
      "public $message_name$ add$capitalized_name$($type$ value) {\n"
      "$null_check$"
      "  if ($name$_.isEmpty()) {\n"
      "    $name$_ = new java.util.ArrayList<$boxed_type$>();\n"
      "  }\n"
      "  $name$_.add(value);\n"
      "  return this;\n"
      "}\n"
      "public $message_name$ clear$capitalized_name$() {\n"
      "  $name$_ = java.util.Collections.emptyList();\n"
      "  return this;\n"
      "}\n");
  }
  if (descriptor_->options().packed()) {
    printer->Print(variables_,
      "private int $name$MemoizedSerializedSize;\n");
  }
}

void RepeatedPrimitiveFieldGenerator::
GenerateMergingCode(io::Printer* printer) const {
  if (params_.java_use_vector()) {
    printer->Print(variables_,
      "if (other.$name$_.size() != 0) {\n"
      "  for (int i = 0; i < other.$name$_.size(); i++)) {\n"
      "    result.$name$_.addElement(other.$name$_.elementAt(i));\n"
      "  }\n"
      "}\n");
  } else {
    printer->Print(variables_,
      "if (!other.$name$_.isEmpty()) {\n"
      "  if (result.$name$_.isEmpty()) {\n"
      "    result.$name$_ = new java.util.ArrayList<$type$>();\n"
      "  }\n"
      "  result.$name$_.addAll(other.$name$_);\n"
      "}\n");
  }
}

void RepeatedPrimitiveFieldGenerator::
GenerateParsingCode(io::Printer* printer) const {
  if (descriptor_->options().packed()) {
    printer->Print(variables_,
      "int length = input.readRawVarint32();\n"
      "int limit = input.pushLimit(length);\n"
      "while (input.getBytesUntilLimit() > 0) {\n"
      "  add$capitalized_name$(input.read$capitalized_type$());\n"
      "}\n"
      "input.popLimit(limit);\n");
  } else {
    printer->Print(variables_,
      "add$capitalized_name$(input.read$capitalized_type$());\n");
  }
}

void RepeatedPrimitiveFieldGenerator::
GenerateSerializationCode(io::Printer* printer) const {
  if (descriptor_->options().packed()) {
    printer->Print(variables_,
      "if (get$capitalized_name$List().size() > 0) {\n"
        "  output.writeRawVarint32($tag$);\n"
        "  output.writeRawVarint32($name$MemoizedSerializedSize);\n"
        "}\n");
    if (params_.java_use_vector()) {
      printer->Print(variables_,
        "for (int i = 0; i < get$capitalized_name$List().size(); i++) {\n"
        "  output.write$capitalized_type$NoTag(get$capitalized_name$(i));\n"
        "}\n");
    } else {
      printer->Print(variables_,
        "for ($type$ element : get$capitalized_name$List()) {\n"
        "  output.write$capitalized_type$NoTag(element);\n"
        "}\n");
    }
  } else {
    if (params_.java_use_vector()) {
      if (IsFastStringHandling(descriptor_, params_)) {
        printer->Print(variables_,
          "for (int i = 0; i < $name$Utf8_.size(); i++) {\n"
          "  output.writeByteArray($number$, (byte []) $name$Utf8_.get(i));\n"
          "}\n");
      } else {
        printer->Print(variables_,
          "for (int i = 0; i < get$capitalized_name$List().size(); i++) {\n"
          "  output.write$capitalized_type$($number$, get$capitalized_name$(i));\n"
          "}\n");
      }
    } else {
      if (IsFastStringHandling(descriptor_, params_)) {
        printer->Print(variables_,
          "for (byte [] element : $name$Utf8_) {\n"
          "  output.writeByteArray($number$, element);\n"
          "}\n");
      } else {
        printer->Print(variables_,
          "for ($type$ element : get$capitalized_name$List()) {\n"
          "  output.write$capitalized_type$($number$, element);\n"
          "}\n");
      }
    }
  }
}

void RepeatedPrimitiveFieldGenerator::
GenerateSerializedSizeCode(io::Printer* printer) const {
  printer->Print(variables_,
    "{\n"
    "  int dataSize = 0;\n");
  printer->Indent();

  if (FixedSize(descriptor_->type()) == -1) {
    if (params_.java_use_vector()) {
      if (IsFastStringHandling(descriptor_, params_)) {
        printer->Print(variables_,
          "$name$Utf8_ = new java.util.Vector();\n"
          "byte[] bytes = null;\n"
          "int sizeArray = get$capitalized_name$List().size();\n"
          "for (int i = 0; i < sizeArray; i++) {\n"
          "  $type$ element = ($type$)$name$_.elementAt(i);\n"
          "  try {\n"
          "    bytes = element.getBytes(\"UTF-8\");\n"
          "  } catch (java.io.UnsupportedEncodingException e) {\n"
          "    throw new RuntimeException(\"UTF-8 not supported.\");\n"
          "  }\n"
          "  $name$Utf8_.addElement(bytes);\n"
          "  dataSize += com.google.protobuf.micro.CodedOutputStreamMicro\n"
          "    .computeByteArraySizeNoTag(bytes);\n"
          "}\n");
      } else {
        printer->Print(variables_,
          "for (int i = 0; i < get$capitalized_name$List().size(); i++) {\n"
          "  dataSize += com.google.protobuf.micro.CodedOutputStreamMicro\n"
          "    .compute$capitalized_type$SizeNoTag(($type$)get$capitalized_name$(i));\n"
          "}\n");
      }
    } else {
      if (IsFastStringHandling(descriptor_, params_)) {
          printer->Print(variables_,
            "$name$Utf8_ = new java.util.ArrayList<byte[]>();\n"
            "byte[] bytes = null;\n"
            "int sizeArray = get$capitalized_name$List().size();\n"
            "for (int i = 0; i < sizeArray; i++) {\n"
            "   $type$ element = get$capitalized_name$(i);\n"
            "  try {\n"
            "    bytes = element.getBytes(\"UTF-8\");\n"
            "  } catch (java.io.UnsupportedEncodingException e) {\n"
            "    throw new RuntimeException(\"UTF-8 not supported.\");\n"
            "  }\n"
            "  $name$Utf8_.add(bytes);\n"
            "  dataSize += com.google.protobuf.micro.CodedOutputStreamMicro\n"
            "    .computeByteArraySizeNoTag(bytes);\n"
            "}\n");
      } else {
        printer->Print(variables_,
            "for ($type$ element : get$capitalized_name$List()) {\n"
            "  dataSize += com.google.protobuf.micro.CodedOutputStreamMicro\n"
            "    .compute$capitalized_type$SizeNoTag(element);\n"
          "}\n");
      }
    }
  } else {
    printer->Print(variables_,
      "dataSize = $fixed_size$ * get$capitalized_name$List().size();\n");
  }

  printer->Print(
      "size += dataSize;\n");

  if (descriptor_->options().packed()) {
    if (params_.java_use_vector()) {
      printer->Print(variables_,
        "if (get$capitalized_name$List().size() != 0) {\n");
    } else {
      printer->Print(variables_,
        "if (!get$capitalized_name$List().isEmpty()) {\n");
    }
    printer->Print(variables_,
        "  size += $tag_size$;\n"
        "  size += com.google.protobuf.micro.CodedOutputStreamMicro\n"
        "      .computeInt32SizeNoTag(dataSize);\n"
        "}\n");
  } else {
    printer->Print(variables_,
      "size += $tag_size$ * get$capitalized_name$List().size();\n");
  }

  // cache the data size for packed fields.
  if (descriptor_->options().packed()) {
    printer->Print(variables_,
      "$name$MemoizedSerializedSize = dataSize;\n");
  }

  printer->Outdent();
  printer->Print("}\n");
}

string RepeatedPrimitiveFieldGenerator::GetBoxedType() const {
  return BoxedPrimitiveTypeName(GetJavaType(descriptor_));
}

}  // namespace javamicro
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
