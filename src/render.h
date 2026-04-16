#pragma once

void drawCrosshair();
void drawGun(unsigned int shaderProgram, unsigned int VAO,
             unsigned int modelLoc, unsigned int viewLoc,
             unsigned int projLoc, unsigned int colorLoc);