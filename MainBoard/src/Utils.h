#pragma once

String convertIntToBeautyString(int integer, int signCount){
  String zeros = "";
  for (int i = 0; i < signCount; i++){
	zeros += "0";
  }
  String source = zeros + String(integer);
  return source.substring(source.length() - signCount, source.length());
}