// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UNREALBOIDS_API BoidUtils
{
public:
	BoidUtils() = default;
	virtual ~BoidUtils() = default;

	/**
	 * Return array of points spread evenly on surface of unit sphere
	 *
	 * @param pointsAmount Amount of points on sphere
	 * @return array of points of sphere
	 */
	static TArray<FVector> GetPointsOnUnitSphere(int pointsAmount);
};
