/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruiz-mo <aruiz-mo@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 19:03:07 by aruiz-mo          #+#    #+#             */
/*   Updated: 2023/11/15 12:25:57 by aruiz-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <iostream>
# include <vector>

void						logMessage(const std::string& message);
std::string 				getPassMsg(std::string msg);
std::string 				getUserMsg(std::string msg);
std::string 				getRealNameMsg(std::string msg);
std::string 				getNickMsg(std::string msg);
std::vector<std::string>	getChannelMsg(std::string msg);