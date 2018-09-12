/*
 * Copyright (C) 2018 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gtest/gtest.h>
#include "sdf/Sphere.hh"

/////////////////////////////////////////////////
TEST(DOMSphere, Construction)
{
  sdf::Sphere sphere;
  EXPECT_EQ(nullptr, sphere.Element());

  EXPECT_DOUBLE_EQ(1.0, sphere.Radius());

  sphere.SetRadius(0.5);

  EXPECT_DOUBLE_EQ(0.5, sphere.Radius());
}

/////////////////////////////////////////////////
TEST(DOMSphere, MoveConstructor)
{
  sdf::Sphere sphere;
  sphere.SetRadius(0.2);

  sdf::Sphere sphere2(std::move(sphere));
  EXPECT_DOUBLE_EQ(0.2, sphere2.Radius());
}

/////////////////////////////////////////////////
TEST(DOMSphere, CopyConstructor)
{
  sdf::Sphere sphere;
  sphere.SetRadius(0.2);

  sdf::Sphere sphere2(sphere);
  EXPECT_DOUBLE_EQ(0.2, sphere2.Radius());
}

/////////////////////////////////////////////////
TEST(DOMSphere, AssignemntOperator)
{
  sdf::Sphere sphere;
  sphere.SetRadius(0.2);

  sdf::Sphere sphere2 = sphere;
  EXPECT_DOUBLE_EQ(0.2, sphere2.Radius());
}

/////////////////////////////////////////////////
TEST(DOMSphere, Load)
{
  sdf::Sphere sphere;
  sdf::Errors errors;

  // Null element name
  errors = sphere.Load(nullptr);
  ASSERT_EQ(1u, errors.size());
  EXPECT_EQ(sdf::ErrorCode::ELEMENT_MISSING, errors[0].Code());
  EXPECT_EQ(nullptr, sphere.Element());

  // Bad element name
  sdf::ElementPtr sdf(new sdf::Element());
  sdf->SetName("bad");
  errors = sphere.Load(sdf);
  ASSERT_EQ(1u, errors.size());
  EXPECT_EQ(sdf::ErrorCode::ELEMENT_INCORRECT_TYPE, errors[0].Code());
  EXPECT_NE(nullptr, sphere.Element());

  // Missing <radius> element
  sdf->SetName("sphere");
  errors = sphere.Load(sdf);
  ASSERT_EQ(1u, errors.size());
  EXPECT_EQ(sdf::ErrorCode::ELEMENT_MISSING, errors[0].Code());
  EXPECT_NE(std::string::npos, errors[0].Message().find("missing a <radius>"));
  EXPECT_NE(nullptr, sphere.Element());
}