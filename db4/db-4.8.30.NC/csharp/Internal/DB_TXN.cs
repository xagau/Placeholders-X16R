/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.38
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace BerkeleyDB.Internal {

using System;
using System.Runtime.InteropServices;

internal class DB_TXN : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal DB_TXN(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(DB_TXN obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~DB_TXN() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        libdb_csharpPINVOKE.delete_DB_TXN(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  internal int abort() {
		int ret;
		ret = libdb_csharpPINVOKE.DB_TXN_abort(swigCPtr);
		DatabaseException.ThrowException(ret);
		return ret;
}

  internal int commit(uint flags) {
		int ret;
		ret = libdb_csharpPINVOKE.DB_TXN_commit(swigCPtr, flags);
		DatabaseException.ThrowException(ret);
		return ret;
}

  internal int discard(uint flags) {
		int ret;
		ret = libdb_csharpPINVOKE.DB_TXN_discard(swigCPtr, flags);
		DatabaseException.ThrowException(ret);
		return ret;
}

  internal uint id() {
    uint ret = libdb_csharpPINVOKE.DB_TXN_id(swigCPtr);
    return ret;
  }

  internal int prepare(byte[] globalID) {
		int ret;
		ret = libdb_csharpPINVOKE.DB_TXN_prepare(swigCPtr, globalID);
		DatabaseException.ThrowException(ret);
		return ret;
}

  internal int get_name(ref string name) {
		int ret;
		ret = libdb_csharpPINVOKE.DB_TXN_get_name(swigCPtr, ref name);
		DatabaseException.ThrowException(ret);
		return ret;
}

  internal int set_name(string name) {
		int ret;
		ret = libdb_csharpPINVOKE.DB_TXN_set_name(swigCPtr, name);
		DatabaseException.ThrowException(ret);
		return ret;
}

  internal int set_timeout(uint timeout, uint flags) {
		int ret;
		ret = libdb_csharpPINVOKE.DB_TXN_set_timeout(swigCPtr, timeout, flags);
		DatabaseException.ThrowException(ret);
		return ret;
}

  internal DB_TXN() : this(libdb_csharpPINVOKE.new_DB_TXN(), true) {
  }

}

}
