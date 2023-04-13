#include <mbgl/test/util.hpp>

#include <mbgl/platform/gl_functions.hpp>

using namespace mbgl::platform;

TEST(GLFunctions, OpenGLES) {
    EXPECT_NE(glActiveTexture, nullptr);
    EXPECT_NE(glAttachShader, nullptr);
    EXPECT_NE(glBindAttribLocation, nullptr);
    EXPECT_NE(glBindBuffer, nullptr);
    EXPECT_NE(glBindFramebuffer, nullptr);
    EXPECT_NE(glBindRenderbuffer, nullptr);
    EXPECT_NE(glBindTexture, nullptr);
    EXPECT_NE(glBlendColor, nullptr);
    EXPECT_NE(glBlendEquation, nullptr);
    EXPECT_NE(glBlendEquationSeparate, nullptr);
    EXPECT_NE(glBlendFunc, nullptr);
    EXPECT_NE(glBlendFuncSeparate, nullptr);
    EXPECT_NE(glBufferData, nullptr);
    EXPECT_NE(glBufferSubData, nullptr);
    EXPECT_NE(glCheckFramebufferStatus, nullptr);
    EXPECT_NE(glClear, nullptr);
    EXPECT_NE(glClearColor, nullptr);
    EXPECT_NE(glClearDepthf, nullptr);
    EXPECT_NE(glClearStencil, nullptr);
    EXPECT_NE(glColorMask, nullptr);
    EXPECT_NE(glCompileShader, nullptr);
    EXPECT_NE(glCompressedTexImage2D, nullptr);
    EXPECT_NE(glCompressedTexSubImage2D, nullptr);
    EXPECT_NE(glCopyTexImage2D, nullptr);
    EXPECT_NE(glCopyTexSubImage2D, nullptr);
    EXPECT_NE(glCreateProgram, nullptr);
    EXPECT_NE(glCreateShader, nullptr);
    EXPECT_NE(glCullFace, nullptr);
    EXPECT_NE(glDeleteBuffers, nullptr);
    EXPECT_NE(glDeleteFramebuffers, nullptr);
    EXPECT_NE(glDeleteProgram, nullptr);
    EXPECT_NE(glDeleteRenderbuffers, nullptr);
    EXPECT_NE(glDeleteShader, nullptr);
    EXPECT_NE(glDeleteTextures, nullptr);
    EXPECT_NE(glDepthFunc, nullptr);
    EXPECT_NE(glDepthMask, nullptr);
    EXPECT_NE(glDepthRangef, nullptr);
    EXPECT_NE(glDetachShader, nullptr);
    EXPECT_NE(glDisable, nullptr);
    EXPECT_NE(glDisableVertexAttribArray, nullptr);
    EXPECT_NE(glDrawArrays, nullptr);
    EXPECT_NE(glDrawElements, nullptr);
    EXPECT_NE(glEnable, nullptr);
    EXPECT_NE(glEnableVertexAttribArray, nullptr);
    EXPECT_NE(glFinish, nullptr);
    EXPECT_NE(glFlush, nullptr);
    EXPECT_NE(glFramebufferRenderbuffer, nullptr);
    EXPECT_NE(glFramebufferTexture2D, nullptr);
    EXPECT_NE(glFrontFace, nullptr);
    EXPECT_NE(glGenBuffers, nullptr);
    EXPECT_NE(glGenerateMipmap, nullptr);
    EXPECT_NE(glGenFramebuffers, nullptr);
    EXPECT_NE(glGenRenderbuffers, nullptr);
    EXPECT_NE(glGenTextures, nullptr);
    EXPECT_NE(glGetActiveAttrib, nullptr);
    EXPECT_NE(glGetActiveUniform, nullptr);
    EXPECT_NE(glGetAttachedShaders, nullptr);
    EXPECT_NE(glGetAttribLocation, nullptr);
    EXPECT_NE(glGetBooleanv, nullptr);
    EXPECT_NE(glGetBufferParameteriv, nullptr);
    EXPECT_NE(glGetError, nullptr);
    EXPECT_NE(glGetFloatv, nullptr);
    EXPECT_NE(glGetFramebufferAttachmentParameteriv, nullptr);
    EXPECT_NE(glGetIntegerv, nullptr);
    EXPECT_NE(glGetProgramInfoLog, nullptr);
    EXPECT_NE(glGetProgramiv, nullptr);
    EXPECT_NE(glGetRenderbufferParameteriv, nullptr);
    EXPECT_NE(glGetShaderInfoLog, nullptr);
    EXPECT_NE(glGetShaderiv, nullptr);
    EXPECT_NE(glGetShaderSource, nullptr);
    EXPECT_NE(glGetString, nullptr);
    EXPECT_NE(glGetTexParameterfv, nullptr);
    EXPECT_NE(glGetTexParameteriv, nullptr);
    EXPECT_NE(glGetUniformfv, nullptr);
    EXPECT_NE(glGetUniformiv, nullptr);
    EXPECT_NE(glGetUniformLocation, nullptr);
    EXPECT_NE(glGetVertexAttribfv, nullptr);
    EXPECT_NE(glGetVertexAttribiv, nullptr);
    EXPECT_NE(glGetVertexAttribPointerv, nullptr);
    EXPECT_NE(glHint, nullptr);
    EXPECT_NE(glIsBuffer, nullptr);
    EXPECT_NE(glIsEnabled, nullptr);
    EXPECT_NE(glIsFramebuffer, nullptr);
    EXPECT_NE(glIsProgram, nullptr);
    EXPECT_NE(glIsRenderbuffer, nullptr);
    EXPECT_NE(glIsShader, nullptr);
    EXPECT_NE(glIsTexture, nullptr);
    EXPECT_NE(glLineWidth, nullptr);
    EXPECT_NE(glLinkProgram, nullptr);
    EXPECT_NE(glPixelStorei, nullptr);
    EXPECT_NE(glPolygonOffset, nullptr);
    EXPECT_NE(glReadPixels, nullptr);
    EXPECT_NE(glRenderbufferStorage, nullptr);
    EXPECT_NE(glSampleCoverage, nullptr);
    EXPECT_NE(glScissor, nullptr);
    EXPECT_NE(glShaderSource, nullptr);
    EXPECT_NE(glStencilFunc, nullptr);
    EXPECT_NE(glStencilFuncSeparate, nullptr);
    EXPECT_NE(glStencilMask, nullptr);
    EXPECT_NE(glStencilMaskSeparate, nullptr);
    EXPECT_NE(glStencilOp, nullptr);
    EXPECT_NE(glStencilOpSeparate, nullptr);
    EXPECT_NE(glTexImage2D, nullptr);
    EXPECT_NE(glTexParameterf, nullptr);
    EXPECT_NE(glTexParameterfv, nullptr);
    EXPECT_NE(glTexParameteri, nullptr);
    EXPECT_NE(glTexParameteriv, nullptr);
    EXPECT_NE(glTexSubImage2D, nullptr);
    EXPECT_NE(glUniform1f, nullptr);
    EXPECT_NE(glUniform1fv, nullptr);
    EXPECT_NE(glUniform1i, nullptr);
    EXPECT_NE(glUniform1iv, nullptr);
    EXPECT_NE(glUniform2f, nullptr);
    EXPECT_NE(glUniform2fv, nullptr);
    EXPECT_NE(glUniform2i, nullptr);
    EXPECT_NE(glUniform2iv, nullptr);
    EXPECT_NE(glUniform3f, nullptr);
    EXPECT_NE(glUniform3fv, nullptr);
    EXPECT_NE(glUniform3i, nullptr);
    EXPECT_NE(glUniform3iv, nullptr);
    EXPECT_NE(glUniform4f, nullptr);
    EXPECT_NE(glUniform4fv, nullptr);
    EXPECT_NE(glUniform4i, nullptr);
    EXPECT_NE(glUniform4iv, nullptr);
    EXPECT_NE(glUniformMatrix2fv, nullptr);
    EXPECT_NE(glUniformMatrix3fv, nullptr);
    EXPECT_NE(glUniformMatrix4fv, nullptr);
    EXPECT_NE(glUseProgram, nullptr);
    EXPECT_NE(glValidateProgram, nullptr);
    EXPECT_NE(glVertexAttrib1f, nullptr);
    EXPECT_NE(glVertexAttrib1fv, nullptr);
    EXPECT_NE(glVertexAttrib2f, nullptr);
    EXPECT_NE(glVertexAttrib2fv, nullptr);
    EXPECT_NE(glVertexAttrib3f, nullptr);
    EXPECT_NE(glVertexAttrib3fv, nullptr);
    EXPECT_NE(glVertexAttrib4f, nullptr);
    EXPECT_NE(glVertexAttrib4fv, nullptr);
    EXPECT_NE(glVertexAttribPointer, nullptr);
    EXPECT_NE(glViewport, nullptr);
}
