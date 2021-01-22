/**
 * Copyright 2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ops/grad/dropout_grad.h"
#include "ops/op_utils.h"

namespace mindspore {
namespace ops {
void DropoutGrad::Init(const float ratio) { set_ratio(ratio); }

void DropoutGrad::set_ratio(const float ratio) {
  CheckAndConvertUtils::CheckInRange<float>(kRatio, ratio, kIncludeRight, {0.0, 1.0}, this->name());
  this->AddAttr(kRatio, MakeValue(ratio));
}

float DropoutGrad::get_ratio() const {
  auto value_ptr = GetAttr(kRatio);
  return GetValue<float>(value_ptr);
}

namespace {
abstract::ShapePtr DropoutGradInferShape(const PrimitivePtr &primitive,
                                         const std::vector<AbstractBasePtr> &input_args) {
  MS_EXCEPTION_IF_NULL(primitive);
  auto DropoutGrad_prim = primitive->cast<PrimDropoutGradPtr>();
  MS_EXCEPTION_IF_NULL(DropoutGrad_prim);
  auto op_name = DropoutGrad_prim->name();
  auto in_shape = CheckAndConvertUtils::ConvertShapePtrToShape("input_shape", input_args[0]->BuildShape(), op_name);
  return std::make_shared<abstract::Shape>(in_shape);
}

TypePtr DropoutGradInferType(const PrimitivePtr &prim, const std::vector<AbstractBasePtr> &input_args) {
  MS_EXCEPTION_IF_NULL(prim);
  auto DropoutGrad_prim = prim->cast<PrimDropoutGradPtr>();
  MS_EXCEPTION_IF_NULL(DropoutGrad_prim);
  auto op_name = DropoutGrad_prim->name();
  auto mask_dtype = input_args[1]->BuildType();
  auto dy_dtype = input_args[0]->BuildType();
  CheckAndConvertUtils::CheckSubClass("mask", mask_dtype, {TypeIdToType(kObjectTypeTensorType)}, op_name);
  CheckAndConvertUtils::CheckTensorTypeValid("dy", dy_dtype, {kNumberTypeFloat16, kNumberTypeFloat32}, op_name);
  auto tensor_type = dy_dtype->cast<TensorTypePtr>();
  MS_EXCEPTION_IF_NULL(tensor_type);
  auto data_type = tensor_type->element();
  return data_type;
}
}  // namespace
AbstractBasePtr DropoutGradInfer(const abstract::AnalysisEnginePtr &, const PrimitivePtr &primitive,
                                 const std::vector<AbstractBasePtr> &input_args) {
  return std::make_shared<abstract::AbstractTensor>(DropoutGradInferType(primitive, input_args),
                                                    DropoutGradInferShape(primitive, input_args)->shape());
}

REGISTER_PRIMITIVE_EVAL_IMPL(DropoutGrad, prim::kPrimDropoutGrad, DropoutGradInfer);
REGISTER_PRIMITIVE_C(kNameDropoutGrad, DropoutGrad);
}  // namespace ops
}  // namespace mindspore