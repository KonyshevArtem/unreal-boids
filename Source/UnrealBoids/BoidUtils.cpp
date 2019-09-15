// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidUtils.h"

TArray<FVector> BoidUtils::GetPointsOnUnitSphere(int pointsAmount)
{
	TArray<FVector> points = TArray<FVector>();

	const float goldenRatio = (1 + FMath::Sqrt(5)) / 2;
	const float angleIncrement = PI * 2 * goldenRatio;

	for (int i = 0; i < pointsAmount; i++) {
		const float t = static_cast<float>(i) / pointsAmount;
		const float inclination = FMath::Acos(1 - 2 * t);
		const float azimuth = angleIncrement * i;

		const float x = FMath::Cos(inclination);
		const float y = FMath::Sin(inclination) * FMath::Sin(azimuth);
		const float z = FMath::Sin(inclination) * FMath::Cos(azimuth);
		points.Add(FVector(x, y, z));
	}
	return points;
}
