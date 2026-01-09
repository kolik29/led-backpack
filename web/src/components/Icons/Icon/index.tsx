import { useEffect, useRef } from "react";
import { drawFrame } from "../../../helpers/Frame";
import Frame from "../../../types/Frame";
import { sendFrame } from "../../../Api/Requests";

const Icon = ({ icon, scale = 10 }: { icon: Frame, scale?: number }) => {
  const ref = useRef<HTMLCanvasElement | null>(null);

  useEffect(() => {
    if (ref.current) {
      drawFrame(ref.current, icon);
    }
  }, [icon, scale]);

  const send = async () => {
    try {
      await sendFrame(icon);
    } catch (e) {
      console.error(e);
    }
  }

  return <>
    <div className='canvas'>
      <canvas
        ref={ref}
        onClick={send}
      ></canvas>
    </div>
  </>
}

export default Icon;