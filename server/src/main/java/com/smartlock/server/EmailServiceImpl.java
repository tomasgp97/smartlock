package com.smartlock.server;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.mail.SimpleMailMessage;
import org.springframework.mail.javamail.JavaMailSender;
import org.springframework.mail.javamail.JavaMailSenderImpl;
import org.springframework.stereotype.Component;

import java.util.Properties;

@Component
public class EmailServiceImpl {

    @Autowired
    public JavaMailSender emailSender;

    /**
     * Send a simple text email
     * @param to email address for the recipient
     * @param subject subject for the email to be sent
     * @param text text contained in the email to be sent
     * @return {@code ResponseEntity}, OK if the email was sent successfully, BAD_REQUEST if not
     */
    public ResponseEntity sendSimpleMessage(String to, String subject, String text) {
        try {
            emailSender = getJavaMailSender();
            SimpleMailMessage message = new SimpleMailMessage();
            message.setTo(to);
            message.setSubject(subject);
            message.setText(text);
            emailSender.send(message);
            return new ResponseEntity(HttpStatus.OK);
        }catch (Exception e){
            return new ResponseEntity(HttpStatus.BAD_REQUEST);
        }
    }

    /**
     * Bean to instantiate the {@code JavaEmailSender} object that will be used to send the email
     * @return {@code JavaEmailSender} the will be used to send the email
     */
    @Bean
    public JavaMailSender getJavaMailSender() {
        JavaMailSenderImpl mailSender = new JavaMailSenderImpl();
        mailSender.setHost("smtp.gmail.com");
        mailSender.setPort(587);

        mailSender.setUsername("smartlockintrocom@gmail.com");
        mailSender.setPassword("introcom1");

        Properties props = mailSender.getJavaMailProperties();
        props.put("mail.transport.protocol", "smtp");
        props.put("mail.smtp.auth", "true");
        props.put("mail.smtp.starttls.enable", "true");
        props.put("mail.debug", "true");

        return mailSender;
    }
}
