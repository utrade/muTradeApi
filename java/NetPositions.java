/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class NetPositions {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected NetPositions(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(NetPositions obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        ContextModeuleJNI.delete_NetPositions(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public Position getPosition(Instrument instrument, Side orderMode) {
    long cPtr = ContextModeuleJNI.NetPositions_getPosition(swigCPtr, this, Instrument.getCPtr(instrument), instrument, orderMode.swigValue());
    return (cPtr == 0) ? null : new Position(cPtr, false);
  }

  public int update(ExecutionReport report) {
    return ContextModeuleJNI.NetPositions_update(swigCPtr, this, ExecutionReport.getCPtr(report), report);
  }

  public NetPositions() {
    this(ContextModeuleJNI.new_NetPositions(), true);
  }

}