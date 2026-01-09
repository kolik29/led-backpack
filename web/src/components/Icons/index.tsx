import ICONS from '../../storage/Icons';
import Frame from '../../types/Frame';
import Icon from './Icon';
import styles from './index.module.sass';

const Icons = () => {
  return <>
    <div className={styles.wrapper}>
      {ICONS.map((icon: Frame, index: number) => <Icon icon={icon} key={index} />)}
    </div>
  </>
};

export default Icons;